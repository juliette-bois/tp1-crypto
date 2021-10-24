//
// Created by Juliette on 16/10/2021.
//

#include <../includes/config.h>
#include <stdio.h>

/**
 *
 * @param alphabet
 * @param min_size
 * @param max_size
 * @param algorithm
 * @return
 */
struct Config* generateConfig(char* alphabet, size_t min_size, size_t max_size, enum algorithm algorithm)
{
    struct Config* config = malloc(sizeof(struct Config));

    config->alphabet = alphabet;
    config->min_size = min_size;
    config->max_size = max_size;
    config->algorithm = algorithm;
    config->T = NULL;
    computeN(config);

    return config;
}

/**
 *
 * @param config
 */
void computeN(struct Config* config)
{
    if (config == NULL) {
        return;
    }
    config->alphabet_size = strlen(config->alphabet);
    if (config->T == NULL) {
        config->T = calloc((config->max_size - (config->min_size - 1)), sizeof(uint64_t));
        config->size_T = config->max_size - (config->min_size - 1);
    } else if (config->size_T < (config->max_size - (config->min_size - 1))) {
        config->T = (uint64_t *) realloc(config->T, (config->max_size - (config->min_size - 1)) * sizeof(uint64_t));
        config->size_T = (config->max_size - (config->min_size - 1));
    }
    uint64_t length =  (uint64_t) config->alphabet_size;
    uint64_t power = length;
    config->N = 0;
    for (size_t i = 1; i < config->min_size; i++) {
        power *= length;
    }
    for (size_t i = config->min_size; i <= config->max_size; i++) {
        config->T[i - config->min_size] = power;
        config->N += config->T[i - config->min_size];
        power *= length;
    }
}

/**
 *
 * @param config
 */
void freeConfig(struct Config** config)
{
    if (*config == NULL) {
        return;
    }
    struct Config* _config = *config;
    if (_config->T) {
        free(_config->T);
    }
    free(*config);
    *config = NULL;
}

/**
 *
 * @param config
 */
void displayConfig(struct Config* config) {
    if (config->algorithm == A_MD5) {
        printf("config : algo : md5\n");
    } else if (config->algorithm == A_SHA1) {
        printf("config : algo : sha1\n");
    } else {
        printf("undefined algorithm\n");
        exit(1);
    }
    printf("config : alphabet size : %li\n", config->alphabet_size);
    printf("config : alphabet : %s\n", config->alphabet);
    printf("config : min : %lu\n", config->min_size);
    printf("config : max : %lu\n", config->max_size);
}