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
    if (TEXT == NULL) {
        TEXT = calloc(wordSize + 1, sizeof(char));
        digest_size = wordSize;
    } else if (digest_size < wordSize) {
        TEXT = (char*) realloc(TEXT, (wordSize + 1) * sizeof(char));
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

char* i2c(int digest, struct Config* config)
{
    size_t i = config->min_size;
    while((i <= config->max_size) && digest >= config->T[i - config->min_size]) {
        digest -= config->T[i - config->min_size];
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
//    printf("%s\n", c);
    byte Hash[MD5_DIGEST_LENGTH];
    H(c, Hash);
//    displayDigest(MD5_DIGEST_LENGTH, Hash);
    return h2i(Hash, i, config);
}

uint64_t new_chain(int idx1, size_t weight, struct Config* config)
{
    uint64_t n = i2i(idx1, 1, config); // i2
    printf("chaine de taille 1 -> %llu\n", n);
    for (size_t i = 2; i <= weight; i++) {
        n = i2i(n, i, config);
        printf("chaine de taille %lu -> ", i);
        printf("%llu\n", n);
    }
    return n;
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