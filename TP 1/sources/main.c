#include <../includes/config.h>
#include <../includes/crypto.h>
#include <time.h>
#include <unistd.h>

/**
 * Print question number
 * @param i : question number
 */
void printQuestion(int i) {
    printf("-------QUESTION %i-------\n\n", i);
}

void help() {
    printf("usage: ./rbt <CMD> [OPTIONS] [ARGS]\n"
           "\n"
           "Available commands:\n"
           "  create <M> <T> <FILENAME>\n"
           "            create the corresponding rainbow tables (M=height, T=width)\n"
           "  info <FILENAME>\n"
           "            display some information about the table from given file\n"
           "  stats <M> <T>\n"
           "            give some information (cover, size) about rainbow tables without\n"
           "            computing them\n"
           "            (M=height, T=width)\n"
           "  crack <H> <FILENAMES> ...\n"
           "            crack the given hash with the rainbow tables\n"
           "  test ...\n"
           "            development tests (\"./rbt test list\" for available tests)\n"
           "\n"
           "Available options:\n"
           "  --md5                 use md5 hash function (default)\n"
           "  --sha1                use sha1 hash function\n"
           "  --alphabet <s>        allowed characters for clear text\n"
           "  --min-size <n>        minimum size of clear text\n"
           "  --max-size <n>        maximum size of clear text\n"
           "  -s <n> / --size <n>   exact size of clear text\n"
           "  -help / -h            this message");
}

void listTest() {
    printf("available tests:\n"
           "  test config                     show configuration\n"
           "  test hash <s1> <s2> ...         compute hash of strings s1, s2, ...\n"
           "  test i2c <i1> <i2> ...          compute i2c(i1), i2c(i2), ...\n"
           "  test h2i <s> <t>                compute h2i(H(s), t)\n"
           "  test i2i <i1> <t1> ...          compute i2i(i1, t1), i2i(i2, t2), ...\n"
           "  test time_i2i [N]               compute average time of single i2i call over N calls\n"
           "  test full_chain <T> <i1> ...    compute (full) chains starting at i1, i2, ...\n"
           "  test FULL_chain <T> <i1> ...    compute (full, with details) chains starting at i1, i2, ...\n"
           "  test chain <T> <i1> ...         compute chains starting at i1, i2, ...\n"
           "  test search <FILENAME> <i>      search the first and last occurences of i in table\n"
           "  test list                       this list");
}

void explodeStringToHexArray(char* hex_str, byte* byte_array) {
    int hex_str_len = strlen(hex_str);
    int i = 0, j = 0;

    if (hex_str_len % 2 == 1)
    {
        if (sscanf(&(hex_str[0]), "%1hhx", &(byte_array[0])) != 1)
        {
            return;
        }

        i = j = 1;
    }

    for (; i < hex_str_len; i+=2, j++)
    {
        if (sscanf(&(hex_str[i]), "%2hhx", &(byte_array[j])) != 1)
        {
            return;
        }
    }
}

void wrongArgumentSize() {
    printf("not enough arguments\n exiting...\n");
    exit(1);
}

void unknownArgument(char* s) {
    printf("unknown argument %s\n exiting...\n", s);
    exit(1);
}

void question1() {
    printQuestion(1);
    byte salut[MD5_DIGEST_LENGTH];
    hash_MD5("Salut", salut);
    printf("#MD5\nSalut -> ");
    displayDigest(MD5_DIGEST_LENGTH, salut);
    printf("\n");

    byte bob[SHA_DIGEST_LENGTH];
    printf("#SHA1\nBob -> ");
    hash_SHA1("Bob", bob);
    displayDigest(SHA_DIGEST_LENGTH, bob);
    printf("\n");
}

void question2() {
    printQuestion(2);
    struct Config* config = generateConfig("abcdefghijklmnopqrstuvwxyz", 4, 4, A_MD5);
    displayConfig(config);
    printf("N -> %llu\n", config->N);
    config->alphabet = "abcdefghijklmnopqrstuvwxyz";
    config->min_size = 2;
    config->max_size = 4;
    computeN(config);
    displayConfig(config);
    printf("N -> %llu\n", config->N);
    config->alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    config->min_size = 4;
    config->max_size = 7;
    computeN(config);
    displayConfig(config);
    printf("N -> %llu\n", config->N);
    freeConfig(&config);
}

void question3() {
    printQuestion(3);
    struct Config* config = generateConfig("abcdefghijklmnopqrstuvwxyz", 5, 5, A_MD5);
    char* word = i2c(1234567, config);
    printf("i2c(1234567) -> %s\n", word);
    config->alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    config->min_size = 4;
    config->max_size = 4;
    computeN(config);
    word = i2c(1234, config);
    printf("i2c -> %s\n", word);
    config->alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    config->min_size = 1;
    config->max_size = 6;
    computeN(config);
    word = i2c(150106454, config);
    printf("i2c(150106454) -> %s\n", word);
    word = i2c(75324, config);
    printf("i2c(75324) -> %s\n", word);
    word = i2c(1651, config);
    printf("i2c(1651) -> %s\n", word);
    word = i2c(4173921, config);
    printf("i2c(4173921) -> %s\n", word);
    freeConfig(&config);
    freeCrypto();
}

void question5() {
    printQuestion(5);
    struct Config* config = generateConfig("abcdefghijklmnopqrstuvwxyz", 4, 5, A_MD5);
    byte oups[MD5_DIGEST_LENGTH];
    hash_MD5("oups", oups);
    printf("MD5(\"oups\") -> ");
    displayDigest(MD5_DIGEST_LENGTH, oups);
    printf("\nh2i(MD5(\"oups\"), 1) -> %llu\n", h2i(oups, 1, config));
    freeConfig(&config);
    freeCrypto();
}

void question6() {
    printQuestion(6);
    struct Config* config = generateConfig("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 4, 5, A_MD5);
    displayConfig(config);
    printf("chaine de taille 1: %llu\n", new_chain(1, 1, config));
    printf("chaine de taille 2: %llu\n", new_chain(1, 2, config));
    printf("chaine de taille 10: %llu\n", new_chain(1, 10, config));
    printf("chaine de taille 100: %llu\n", new_chain(1, 100, config));
    printf("chaine de taille 1000: %llu\n", new_chain(1, 1000, config));
    freeConfig(&config);
    freeCrypto();
}

void question8() {
    printQuestion(8);
    struct Config* config = generateConfig("abcdefghijklmnopqrstuvwxyz", 5, 5, A_MD5);
    struct table* table = create_table(200, 100, config);
    displayTable(table, 1);
    freeConfig(&config);
    freeCrypto();
    freeTable(&table);
}

void question9() {
    printQuestion(9);
    struct Config* config = generateConfig("abcdefghijklmnopqrstuvwxyz", 5, 5, A_MD5);
    struct table* table = create_table(200, 100, config);
    saveTable(table, "question9");
    freeTable(&table);
    printf("table saved in file question9 and free\n");
    table = openTable("question9");
    printf("table loaded from file question9\n");
    displayTable(table, 0);
    freeConfig(&config);
    freeCrypto();
    freeTable(&table);
}

void question12() {
    printQuestion(12);
    struct Config* config = generateConfig("abcdefghijklmnopqrstuvwxyz", 5, 5, A_MD5);
    struct table* table = create_table(200, 100, config);
    displayTable(table, 0);
    displaySizeAndCoverage(table->width, table->height, table->config->N);
    freeConfig(&config);
    freeCrypto();
    freeTable(&table);
}

void question13() {
    printQuestion(13);
    struct Config* config;
    struct table* table;
    if(access("question13md5", F_OK) == 0) {
        table = openTable("question13md5");
        config = table->config;
    } else {
        printf("generating table for md5 W:1000 H:10000...\n");
        config = generateConfig("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 4, 4, A_MD5);
        table = create_table(1000, 10000, config);
        saveTable(table, "question13md5");
    }
    displayTable(table, 0);
    displaySizeAndCoverage(table->width, table->height, table->config->N);
    char* testmd5[] = {"ABCD", "DCBA", "AZER"};
    byte digestmd5[3][MD5_DIGEST_LENGTH];
    char resultmd5[5];
    for (int i = 0; i < 3; i++) {
        hash_MD5(testmd5[i], digestmd5[i]);
        printf("%s -> ", testmd5[i]);
        displayDigest(MD5_DIGEST_LENGTH, digestmd5[i]);
        printf("\n");
        if (inverse(table, digestmd5[i], resultmd5) != 0) {
            printf("found inverse : %s\n", resultmd5);
        } else {
            printf("inverse not found\n");
        }
    }
    byte hexamd5[MD5_DIGEST_LENGTH];
    explodeStringToHexArray("08054846bbc9933fd0395f8be516a9f9", hexamd5);
    inverse(table, hexamd5, resultmd5);
    printf("%s -> %s\n", "08054846bbc9933fd0395f8be516a9f9", resultmd5);
    freeTable(&table);

    if(access("question13sha1", F_OK) == 0) {
        table = openTable("question13sha1");
    } else {
        printf("generating table for sha1 W:10000 H:50000...\n");
        config->alphabet = "abcdefghijklmnopqrstuvwxyz0123456789,;:$";
        config->min_size = 4;
        config->max_size = 5;
        config->algorithm = A_SHA1;
        computeN(config);
        table = create_table(10000, 50000, config);
        saveTable(table, "question13sha1");
    }
    displayTable(table, 0);
    displaySizeAndCoverage(table->width, table->height, table->config->N);
    char* testsha[] = {"3,141", "abcde", "edcba", "$123;"};
    byte digestsha[4][SHA_DIGEST_LENGTH];
    char resultsha[6];
    for (int i = 0; i < 4; i++) {
        hash_SHA1(testsha[i], digestsha[i]);
        printf("%s -> ", testsha[i]);
        displayDigest(SHA_DIGEST_LENGTH, digestsha[i]);
        printf("\n");
        if (inverse(table, digestsha[i], resultsha) != 0) {
            printf("found inverse : %s\n", resultsha);
        } else {
            printf("inverse not found\n");
        }
    }
    byte hexasha[SHA_DIGEST_LENGTH];
    explodeStringToHexArray("73f9eda0245be0e3a288533c42db1f49f20b0e23", hexasha);
    inverse(table, hexasha, resultsha);
    printf("%s -> %s\n", "73f9eda0245be0e3a288533c42db1f49f20b0e23", resultsha);

    freeConfig(&config);
    freeCrypto();
    freeTable(&table);
}

void question14() {
    printQuestion(14);
    struct Config* config = generateConfig("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 4, 4, A_MD5);
    clock_t begin = clock();
    struct table* table = create_table(1000, 10000, config);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    displayTable(table, 0);
    displaySizeAndCoverage(table->width, table->height, config->N);
    printf("took %lf seconds to generate table\n", time_spent);
    freeConfig(&config);
    freeCrypto();
    freeTable(&table);
}

void question15() {
    printQuestion(15);
    struct Config* config = generateConfig("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 4, 4, A_MD5);
    struct table* table = create_table(1000, 10000, config);
    displayTable(table, 0);
    displaySizeAndCoverage(table->width, table->height, table->config->N);
    char* testmd5[] = {"ABCD", "DCBA", "AZER"};
    byte digestmd5[3][MD5_DIGEST_LENGTH];
    char resultmd5[5];
    for (int i = 0; i < 3; i++) {
        hash_MD5(testmd5[i], digestmd5[i]);
        printf("%s -> ", testmd5[i]);
        displayDigest(MD5_DIGEST_LENGTH, digestmd5[i]);
        printf("\n");
        clock_t begin = clock();
        if (inverse(table, digestmd5[i], resultmd5) != 0) {
            printf("found inverse : %s\n", resultmd5);
        } else {
            printf("inverse not found\n");
        }
        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("took %lf seconds to find result\n", time_spent);
    }
    byte hexamd5[MD5_DIGEST_LENGTH];
    explodeStringToHexArray("08054846bbc9933fd0395f8be516a9f9", hexamd5);
    clock_t begin = clock();
    inverse(table, hexamd5, resultmd5);
    printf("%s -> %s\n", "08054846bbc9933fd0395f8be516a9f9", resultmd5);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("took %lf seconds to find result\n", time_spent);
    freeConfig(&config);
    freeCrypto();
    freeTable(&table);
}

/**
 *
 * @param argc
 * @param argv
 * @return 0 (success) | non null number (error)
 */
int main(int argc, char **argv) {
    srand(time(NULL));

    // help
    if (argc <= 1 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0) {
        help();
        return 0;
    }

    enum algorithm algorithm = A_MD5;
    char *alphabet, *command, *filename, *strhash;
    alphabet = "abcdefghijklmnopqrstuvwxyz";
    size_t minSize = 5, maxSize = 5, height = 100, width = 200;
    for (int i = 1; i < argc; i++) {
        // arguments
        if (argv[i][0] == '-') {
            if (strcmp(argv[i], "--md5") == 0) {
                algorithm = A_MD5;
            } else if (strcmp(argv[i], "--sha1") == 0) {
                algorithm = A_SHA1;
            } else if (strstr(argv[i], "--alphabet") != NULL) {
                alphabet = (argv[i]+11);
            } else if (strstr(argv[i], "--min-size") != NULL) {
                minSize = atol(argv[i]+11);
            } else if (strstr(argv[i], "--max-size") != NULL) {
                maxSize = atol(argv[i]+11);
            } else if (strstr(argv[i], "--size") != NULL) {
                maxSize = minSize = atol(argv[i]+7);
            } else if (strstr(argv[i], "--s") != NULL) {
                maxSize = minSize = atol(argv[i]+4);
            } else {
                unknownArgument(argv[i]);
            }
        } else {
            // commands
            if (strcmp(argv[i], "create") == 0) {
                if (argc < 5) {
                    wrongArgumentSize();
                }
                command = argv[i];
                i++;
                height = atol(argv[i]);
                i++;
                width = atol(argv[i]);
                i++;
                filename = argv[i];
            } else if (strcmp(argv[i], "info") == 0) {
                if (argc < 3) {
                    wrongArgumentSize();
                }
                command = argv[i];
                i++;
                filename = argv[i];
            } else if (strcmp(argv[i], "crack") == 0) {
                if (argc < 4) {
                    wrongArgumentSize();
                }
                command = argv[i];
                i++;
                strhash = argv[i];
                i++;
                filename = argv[i];
            } else if (strcmp(argv[i], "test") == 0) {
                if (argc < 3) {
                    wrongArgumentSize();
                }
                command = argv[i];
                i++;
                if (strcmp(argv[i], "list") == 0) {
                    listTest();
                    return 0;
                }
                struct Config* config = generateConfig(alphabet, minSize, maxSize, algorithm);
                if (strcmp(argv[i], "config") == 0) {
                    displayConfig(config);
                    return 0;
                } else if (strcmp(argv[i], "hash") == 0) {
                    displayConfig(config);
                    i++;
                    byte hash[getDigestSize(config)];
                    for (int j = i; j < argc; j++) {
                        printf("%s -> ", argv[j]);
                        H(argv[j], hash, config);
                        displayDigest(getDigestSize(config), hash);
                        printf("\n");
                    }
                    return 0;
                } else if (strcmp(argv[i], "i2c") == 0) {
                    displayConfig(config);
                    i++;
                    for (int j = i; j < argc; j++) {
                        size_t k = atol(argv[j]);
                        printf("i2c(%li) -> %s", k, i2c(k, config));
                        printf("\n");
                    }
                    return 0;
                } else if (strcmp(argv[i], "h2i") == 0) {
                    displayConfig(config);
                    i++;
                    char* str = argv[i];
                    i++;
                    size_t t = atol(argv[i]);
                    byte hash[getDigestSize(config)];
                    H(str, hash, config);
                    printf("h2i(H(%s), %li) -> %lli\n", str, t, h2i(hash,t, config));
                    return 0;
                } else if (strcmp(argv[i], "i2i") == 0) {
                    displayConfig(config);
                    i++;
                    for (int j = i; j < argc; j++) {
                        size_t k = atol(argv[j]);
                        j++;
                        size_t l = atol(argv[j]);
                        printf("i2i(%li, %li) -> %lli\n", k, l, i2i(k, l, config));
                    }
                    return 0;
                } else if (strcmp(argv[i], "time_i2i") == 0) {
                    struct table* table = create_table(width, height, config);
                    displayTable(table, 0);
                    i++;
                    size_t N = atol(argv[i]);
                    clock_t begin = clock();
                    for (size_t j = 0; j < N; j++) {
                        printf("i2i(%li, %li) -> %lli\n", j, width, i2i(j, width, config));
                    }
                    clock_t end = clock();
                    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
                    printf("total time : %lf sec\naverage per i2i : %lf sec\n", time_spent, time_spent / (double) N);
                    return 0;
                } else if (strcmp(argv[i], "full_chain") == 0) {
                    displayConfig(config);
                    i++;
                    size_t t = atol(argv[i]);
                    i++;
                    for (int j = i; j < argc; j++) {
                        uint64_t idx = atol(argv[j]);
                        printf("chaine de taille 1 -> %llu\n", idx);
                        for (size_t k = 2; k <= t; k++) {
                            idx = i2i(idx, k - 1, config);
                            printf("chaine de taille %lu -> %llu\n", k, idx);
                        }
                    }
                    return 0;
                } else if (strcmp(argv[i], "FULL_chain") == 0) {
                    displayConfig(config);
                    i++;
                    size_t t = atol(argv[i]);
                    i++;
                    for (int j = i; j < argc; j++) {
                        uint64_t idx = atol(argv[j]);
                        printf("chaine de taille 1 -> %lli\n", idx);
                        for (size_t k = 2; k <= t; k++) {
                            char* c = i2c(idx, config);
                            printf("--i2c-->  %s  --h--> ", c);
                            byte Hash[getDigestSize(config)];
                            H(c, Hash, config);
                            displayDigest(getDigestSize(config), Hash);
                            idx = h2i(Hash, k - 1, config);
                            printf(" --h2i(1)--> %lli\n", idx);
                        }
                    }
                    return 0;
                } else if (strcmp(argv[i], "chain") == 0) {
                    displayConfig(config);
                    i++;
                    for (int j = i; j < argc; j++) {
                        size_t t = atol(argv[j]);
                        j++;
                        size_t idx = atol(argv[j]);
                        new_chain(idx, t, config);
                    }
                    return 0;
                } else if (strcmp(argv[i], "search") == 0) {
                    filename = argv[i+1];
                    size_t value = atol(argv[i+2]);
                    printf("opening table from file %s...\n", filename);
                    struct table *table = openTable(filename);
                    printf("opening table from file %s done\n", filename);
                    int a, b;
                    if(search(table, value, &a, &b)) {
                        if (a == b) {
                            printf("found %li in table only once at index %i\n", value, a);
                        } else {
                            printf("found %li in table starting at index %i and ending at index %i\n", value, a, b);
                        }
                    } else {
                        printf("value not found\n");
                    }
                    return 0;
                } else {
                    unknownArgument(argv[i]);
                }
            }
        }
    }

    if (strcmp(command, "create") == 0) {
        struct Config* config = generateConfig(alphabet, minSize, maxSize, algorithm);
        printf("generating table...\n");
        struct table *table = create_table(width, height, config);
        printf("generating table done\n");
        printf("saving table to file %s...\n", filename);
        saveTable(table, filename);
        printf("saving table to file %s done\n", filename);
        freeConfig(&config);
        freeCrypto();
        freeTable(&table);
        return 0;
    } else if (strcmp(command, "info") == 0) {
        printf("opening table from file %s...\n", filename);
        struct table *table = openTable(filename);
        printf("opening table from file %s done\n", filename);
        displayTable(table, 0);
        freeCrypto();
        freeTable(&table);
    } else if (strcmp(command, "stats") == 0) {
        struct Config* config = generateConfig(alphabet, minSize, maxSize, algorithm);
        displaySizeAndCoverage(width, height, config->N);
        freeConfig(&config);
    } else if (strcmp(command, "crack") == 0) {
        struct Config* config = generateConfig(alphabet, minSize, maxSize, algorithm);
        printf("opening table from file %s...\n", filename);
        struct table *table = openTable(filename);
        printf("opening table from file %s done\n", filename);
        byte hash[getDigestSize(config)];
        explodeStringToHexArray(strhash, hash);
        char clear[config->max_size + 1];
        printf("start looking for inverse of ");
        displayDigest(getDigestSize(config), hash);
        printf("...\n");
        if (inverse(table, hash, clear)) {
            printf("found a result : \n");
            displayDigest(getDigestSize(config), hash);
            printf(" -> %s\n", clear);
        } else {
            printf("didn't found a result with given table\n");
        }
    }

    return 0;
}