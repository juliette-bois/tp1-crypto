//
// Created by Juliette on 17/10/2021.
//

#include <../includes/crypto.h>

char* DIGEST;
size_t digest_size = 0;
char* TEXT;

void hash_MD5(const char* s, byte* digest)
{
    MD5((unsigned char*)s, strlen(s), digest);
}

void hash_SHA1(const char* s, byte* digest)
{
    SHA1((unsigned char*)s, strlen(s), digest);
}

void H(const char* s, byte* digest)
{
    if (digest == NULL) {
        digest = calloc(MD5_DIGEST_LENGTH, sizeof(byte));
    }
    hash_MD5(s, digest);
//    byte* H = calloc(SHA_DIGEST_LENGTH, sizeof(byte));
//    hash_SHA1(s, digest);
}

void displayDigest(const int length, byte* digest)
{
    for (int i = 0; i < length; i++) {
        printf("%02X", digest[i]);
    }
    printf("\n");
}

char* computeI2C(int digest, struct Config* config, const int wordSize)
{
    if (DIGEST == NULL) {
        DIGEST = calloc(wordSize, sizeof(char));
        digest_size = wordSize;
    } else if (digest_size < wordSize) {
        DIGEST = (char*) realloc(DIGEST, wordSize * sizeof(char));
        digest_size = wordSize;
    }
    int i = wordSize - 1;
    DIGEST[i] = config->alphabet[digest % config->alphabet_size];
    i--;
    while (i >= 0) {
        digest = digest / config->alphabet_size;
        DIGEST[i] = config->alphabet[digest % config->alphabet_size];
        i--;
    }

    return DIGEST;
}

char* i2c(int digest, struct Config* config)
{
    size_t i = config->min_size;
    while((i <= config->max_size) && digest >= config->T[i - 1]) {
        digest -= config->T[i - 1];
        i++;
    }
    return computeI2C(digest, config, i);
}

uint64_t h2i(byte* digest, int t, struct Config* config)
{
    uint64_t* y = (uint64_t*) digest;
    return (*y + t) % config->N;
}

uint64_t i2i(int digest, int i, struct Config* config)
{
    char* c = i2c(digest, config);
    byte* Hash;
    H(c, Hash);
    return h2i(Hash, i, config);
}

uint64_t nouvelle_chaine(int idx1, size_t largeur, struct Config* config)
{
    uint64_t i1 = i2i(idx1, 1, config);
    uint64_t n = i1;
    for (size_t i = 2; i <= largeur; i++) {
        n =  = i2i(n, i, config);
    }
    return n;
    // nouvelle chaine on doit retourner une chaine de caractère ?
}

void freeCrypto()
{
    if (DIGEST) {
        free(DIGEST);
    }
    if (TEXT) {
        free(TEXT);
    }
}