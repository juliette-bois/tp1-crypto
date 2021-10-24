//
// Created by Juliette on 17/10/2021.
//

#include <../includes/crypto.h>

char* DIGEST;
size_t digest_size = 0;
char* TEXT;

/**
 *
 * @param s
 * @param digest
 */
void hash_MD5(const char* s, byte* digest)
{
    MD5((unsigned char*)s, strlen(s), digest);
}

/**
 *
 * @param s
 * @param digest
 */
void hash_SHA1(const char* s, byte* digest)
{
    SHA1((unsigned char*)s, strlen(s), digest);
}

/**
 *
 * @param s
 * @param digest
 * @param config
 */
void H(const char* s, byte* digest, struct Config* config)
{
    if (config->algorithm == A_MD5) {
        hash_MD5(s, digest);
    } else if (config->algorithm == A_SHA1) {
        hash_SHA1(s, digest);
    }
}

/**
 *
 * @param config
 * @return
 */
int getDigestSize(struct Config* config) {
    if (config->algorithm == A_MD5) {
        return MD5_DIGEST_LENGTH;
    } else {
        return SHA_DIGEST_LENGTH;
    }
}

/**
 *
 * @param length
 * @param digest
 */
void displayDigest(const int length, byte* digest)
{
    for (int i = 0; i < length; i++) {
        printf("%02X", digest[i]);
    }
}

/**
 *
 * @param digest
 * @param config
 * @param wordSize
 * @return
 */
char* computeI2C(int digest, struct Config* config, const int wordSize)
{
    if (TEXT == NULL) {
        TEXT = malloc(wordSize + 1);
        digest_size = wordSize;
    } else if (digest_size < wordSize) {
        TEXT = (char*) realloc(TEXT, (wordSize + 1));
        digest_size = wordSize;
    }

    TEXT[wordSize] = '\0';
    TEXT[wordSize - 1] = config->alphabet[digest % config->alphabet_size];

    for (int i = wordSize - 2; i >= 0; i--) {
        digest = digest / config->alphabet_size;
        TEXT[i] = config->alphabet[digest % config->alphabet_size];
    }

    return TEXT;
}

/**
 *
 * @param digest
 * @param config
 * @return
 */
char* i2c(size_t digest, struct Config* config)
{
    size_t i = config->min_size;
    while((i < config->max_size) && digest >= config->T[i - config->min_size]) {
        digest -= config->T[i - config->min_size];
        i++;
    }
    return computeI2C(digest, config, i);
}

/**
 *
 * @param digest
 * @param t
 * @param config
 * @return
 */
uint64_t h2i(byte* digest, size_t t, struct Config* config)
{
    uint64_t* y = (uint64_t*) digest;
    return (*y + t) % config->N;
}

/**
 *
 * @param digest
 * @param i
 * @param config
 * @return
 */
uint64_t i2i(size_t digest, size_t i, struct Config* config)
{
    char* c = i2c(digest, config);
//    printf("%s\n", c);
    byte Hash[getDigestSize(config)];
    H(c, Hash, config);
//    displayDigest(getDigestSize(config), Hash);
    return h2i(Hash, i, config);
}

/**
 *
 * @param idx1
 * @param weight
 * @param config
 * @return
 */
uint64_t new_chain(int idx1, size_t weight, struct Config* config)
{
    uint64_t n = idx1;
    for (size_t i = 2; i <= weight; i++) {
        n = i2i(n, i - 1, config);
    }
    return n;
}

/**
 *
 */
void freeCrypto()
{
    if (DIGEST != NULL) {
        free(DIGEST);
    }
    if (TEXT != NULL) {
        free(TEXT);
    }
}

/**
 *
 * @return int
 */
int random_index() {
    return rand();
}

/**
 *
 * @param elem1
 * @param elem2
 * @return
 */
int comp(const void * elem1, const void * elem2)
{
    size_t* f = (*(size_t**)elem1);
    size_t* s = (*(size_t**)elem2);
    if (f[1] > s[1]) return  1;
    if (f[1] < s[1]) return -1;
    return 0;
}

/**
 *
 * @param width
 * @param height
 * @param config
 * @return
 */
struct table* create_table(size_t width, size_t height, struct Config* config)
{
    struct table* table = malloc(sizeof(struct table));
    table->width = width;
    table->height = height;
    table->config = config;
    table->table = (size_t **) calloc(height, sizeof(size_t*));
    for(int i = 0; i < height; i++) {
        table->table[i] = (size_t *) calloc(2, sizeof(size_t));
        table->table[i][0] = i + 1;
        table->table[i][1] = new_chain(i + 1, width, config); // for non random table
//        table->table[i][1] = new_chain(random_index() % config->N, width, config);
    }

    qsort(table->table, height, sizeof(*table->table), comp);
    return table;
}

/**
 *
 * @param table
 */
void freeTable(struct table** table) {
    if (*table == NULL) {
        return;
    }
//    freeConfig(&(*table)->config);
//    (*table)->config = NULL;
    for(int i = 0; i < (*table)->height; i++) {
        free((*table)->table[i]);
    }
    free((*table)->table);
    free(*table);
    *table = NULL;
}

/**
 *
 * @param table
 * @param filename
 */
void saveTable(struct table* table, char* filename) {
    FILE* file = fopen(filename, "wb+");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier");
        exit(1);
    }

    if (fwrite(&table->config->algorithm, sizeof(table->config->algorithm), 1, file) <= 0) {
        printf("Impossible d'écrire dans le fichier");
        exit(1);
    }
    if (fwrite(&table->config->alphabet_size, sizeof(table->config->alphabet_size), 1, file) <= 0) {
        printf("Impossible d'écrire dans le fichier");
        exit(1);
    }
    if (fwrite(table->config->alphabet, 1, strlen(table->config->alphabet) + 1, file) <= 0) {
        printf("Impossible d'écrire dans le fichier");
        exit(1);
    }
    if (fwrite(&table->config->min_size, sizeof(table->config->min_size), 1, file) <= 0) {
        printf("Impossible d'écrire dans le fichier");
        exit(1);
    }
    if (fwrite(&table->config->max_size, sizeof(table->config->max_size), 1, file) <= 0) {
        printf("Impossible d'écrire dans le fichier");
        exit(1);
    }
    if (fwrite(&table->width, sizeof(table->width), 1, file) <= 0) {
        printf("Impossible d'écrire dans le fichier");
        exit(1);
    }
    if (fwrite(&table->height, sizeof(table->height), 1, file) <= 0) {
        printf("Impossible d'écrire dans le fichier");
        exit(1);
    }

    for(size_t i = 0; i < table->height; i++) {
        fwrite(&table->table[i][0], sizeof(size_t), 1, file);
        fwrite(&table->table[i][1], sizeof(size_t), 1, file);
    }

    fclose(file);
}

/**
 *
 * @param filename
 * @return
 */
struct table* openTable(char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier");
        exit(1);
    }

    // read header
    struct Config* config = malloc(sizeof(struct Config));
    struct table* table = malloc(sizeof(struct table));

    // algorithm
    if (fread(&config->algorithm, sizeof(enum algorithm), 1, file) <= 0) {
        printf("Couldn't read properly the file");
        fclose(file);
        exit(1);
    }
    // alphabet
    if(fread(&config->alphabet_size, sizeof(size_t), 1, file) <= 0) {
        printf("Couldn't read properly the file");
        fclose(file);
        exit(1);
    }
    config->alphabet = malloc(config->alphabet_size);
    if (fread(config->alphabet, 1, config->alphabet_size + 1, file) <= 0) {
        printf("Couldn't read properly the file");
        fclose(file);
        exit(1);
    }
    // min size
    if (fread(&config->min_size, sizeof(size_t), 1, file) <= 0) {
        printf("Couldn't read properly the file");
        fclose(file);
        exit(1);
    }
    // max size
    if (fread(&config->max_size, sizeof(size_t), 1, file) <= 0) {
        printf("Couldn't read properly the file");
        fclose(file);
        exit(1);
    }
    // width
    if (fread(&table->width, sizeof(size_t), 1, file) <= 0) {
        printf("Couldn't read properly the file");
        fclose(file);
        exit(1);
    }
    // height
    if (fread(&table->height, sizeof(size_t), 1, file) <= 0) {
        printf("Couldn't read properly the file");
        fclose(file);
        exit(1);
    }
    table->config = config;
    computeN(table->config);

    // read table
    table->table = (size_t **) calloc(table->height, sizeof(size_t*));
    for (unsigned int i = 0; i < table->height; i++) {
        table->table[i] = (size_t *) calloc(2, sizeof(size_t));
        if (fread(&table->table[i][0], sizeof(size_t), 1, file) <= 0) {
            printf("Couldn't read properly the file");
            fclose(file);
            exit(1);
        }
        if (fread(&table->table[i][1], sizeof(size_t), 1, file) <= 0) {
            printf("Couldn't read properly the file");
            fclose(file);
            exit(1);
        }
    }

    fclose(file);

    return table;
}

/**
 *
 * @param table
 * @param displaytable
 */
void displayTable(struct table* table, int displaytable)
{
    if (table->config->algorithm == A_MD5) {
        printf("config : algo : md5\n");
    } else if (table->config->algorithm == A_SHA1) {
        printf("config : algo : sha1\n");
    } else {
        printf("undefined algorithm\n");
        exit(1);
    }
    printf("config : alphabet size : %li\n", table->config->alphabet_size);
    printf("config : alphabet : %s\n", table->config->alphabet);
    printf("config : min : %lu\n", table->config->min_size);
    printf("config : max : %lu\n", table->config->max_size);
    printf("table : width : %lu\n", table->width);
    printf("table : height : %lu\n", table->height);
    if (displaytable) {
        for (int i = 0; i < table->height; i++) {
            printf("%lu --> ", table->table[i][0]);
            printf("%lu\n", table->table[i][1]);
        }
    }
}

// essaie d'inverser l'empreinte h
//   - table : table arc-en-ciel
//   - h/digest : empreinte à inverser
//   - clair : (résultat) texte clair dont l'empreinte est h
/**
 *
 * @param table
 * @param digest
 * @param clear
 * @return
 */
int inverse(struct table* table, byte* digest, char* clear)
{
    int nb_candidate = 0;
    for (size_t t = table->width - 1; t > 0; t--) {
        uint64_t idx = h2i(digest, t, table->config);
        for (size_t i = t + 1; i < table->width; i++) {
            idx = i2i(idx, i, table->config);
        }
        int a, b;
        if (search(table, idx, &a, &b) > 0) {
            for (int i = a; i <= b; i++) {
                if (verify_candidate(digest, t, table->table[i][0], clear, table->config) == 1) {
                    return 1;
                } else {
                    nb_candidate++;
                }
            }
        }
    }
    return 0;
}

/**
 *
 * @param arr
 * @param l
 * @param r
 * @param x
 * @return
 */
int binarySearch(size_t** arr, int l, int r, uint64_t x)
{
    if (r >= l) {
        int mid = l + (r - l) / 2;

        // If the element is present at the middle
        // itself
        if (arr[mid][1] == x) {
            return mid;
        }

        // If element is smaller than mid, then
        // it can only be present in left subarray
        if (arr[mid][1] > x) {
            return binarySearch(arr, l, mid - 1, x);
        }

        // Else the element can only be present
        // in right subarray
        return binarySearch(arr, mid + 1, r, x);
    }

    // We reach here when element is not
    // present in array
    return -1;
}

// recherche dichotomique dans la table les premières et dernières lignes dont
// la seconde colonne est égale à idx
//   - table : table arc-en-ciel
//   - hauteur : nombre de chaines dans la table
//   - idx : indice à rechercher dans la dernière (deuxième) colonne
//   - a et b : (résultats) numéros des premières et dernières lignes dont les
//     dernières colonnes sont égale à idx
/**
 *
 * @param table
 * @param idx
 * @param a
 * @param b
 * @return
 */
int search(struct table* table, uint64_t idx, int* a, int* b)
{
    int found = binarySearch(table->table, 0, table->height - 1, idx);
    if (found != -1) {
        int i = found;
        while (i >= 0 && table->table[i][1] == idx) {
            i--;
        }
        i++;
        *a = i;
        i = found;
        while (i < table->height && table->table[i][1] == idx) {
            i++;
        }
        i--;
        *b = i;
        return 1;
    }
    return 0;
}

// vérifie si un candidat est correct
//   - h/digest : empreinte à inverser
//   - t : numéro de la colonne où a été trouvé le candidat
//   - idx : indice candidat (de la colonne t)
//   - clair : résultat : contient le texte clair obtenu
//   - config : config de la table
/**
 *
 * @param digest
 * @param t
 * @param idx
 * @param clear
 * @param config
 * @return
 */
int verify_candidate(byte* digest, int t, int idx, char* clear, struct Config* config)
{
    for (int i = 1; i < t; i++) {
        idx = i2i(idx, i, config);
    }
    char* result = i2c(idx, config);
    memcpy(clear, result, strlen(result));
    byte h2[getDigestSize(config)];
    H(clear, h2, config);
    return memcmp(h2, digest, getDigestSize(config)) == 0;
}

/**
 *
 * @param N
 * @param width
 * @param height
 * @return
 */
float coverage(size_t N, size_t width, size_t height)
{
    double m = height;
    double v = 1.0;
    for (int i = 0; i < width; i++) {
        v = v * (1 - m / N);
        m = N * (1 - exp(-m / N));
    }
    return 100 * (1-v);
}

/**
 *
 * @param width
 * @param height
 * @param N
 */
void displaySizeAndCoverage(const size_t width, const size_t height, uint64_t N)
{
    printf("size of table in bytes : %li\n", height * (sizeof(size_t) * 2));
    printf("coverage of table : %.2f%%\n", coverage(N, width, height));
}