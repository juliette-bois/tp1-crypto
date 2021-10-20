#include <../includes/config.h>
#include <../includes/crypto.h>

int main(int argc, char **argv) {
    // Question 1
    byte* salut = calloc(MD5_DIGEST_LENGTH, sizeof(byte));
    hash_MD5("Salut", salut);
    printf("#MD5\nSalut -> ");
    displayDigest(MD5_DIGEST_LENGTH, salut);

    byte* bob = calloc(SHA_DIGEST_LENGTH, sizeof(byte));
    printf("#SHA1\nBob -> ");
    hash_SHA1("Bob", bob);
    displayDigest(SHA_DIGEST_LENGTH, bob);

    free(salut);
    free(bob);

    // Question 2
    struct Config* config = generateConfig("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 4, 7);
    computeN(config);
    printf("N -> %llu\n", config->N);

    // Question 3
    config->alphabet = "abcdefghijklmnopqrstuvwxyz";
    config->min_size = 5;
    config->max_size = 5;
    computeN(config);
    char* word = i2c(1234567, config);
    printf("i2c -> %s\n", word);

    // Question 5
    config->alphabet = "abcdefghijklmnopqrstuvwxyz";
    config->min_size = 4;
    config->max_size = 5;
    computeN(config);
    byte* oups = calloc(MD5_DIGEST_LENGTH, sizeof(byte));
    hash_MD5("oups", oups);
    // displayDigest(MD5_DIGEST_LENGTH, oups);
    printf("h2i -> %llu\n", h2i(oups, 1, config));
    free(oups);

    // Question 6
    config->alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    config->min_size = 4;
    config->max_size = 5;
    computeN(config);
    new_chain(1, 5, config);

    // Question 7

    freeConfig(config);
    freeCrypto();

    return 0;
}