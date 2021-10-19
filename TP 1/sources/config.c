//
// Created by Juliette on 16/10/2021.
//

#include <../includes/config.h>
#include <stdio.h>

struct Config* generateConfig(char* alphabet, size_t min_size, size_t max_size)
{
    struct Config* config = malloc(sizeof(struct Config));

    config->alphabet = alphabet;
    config->min_size = min_size;
    config->max_size = max_size;

    return config;
}

void computeN(struct Config* config)
{
    if (config == NULL) {
        return;
    }
    config->N = 0;
    config->alphabet_size = strlen(config->alphabet);
    if (config->T == NULL) {
        config->T = calloc(config->max_size - (config->min_size - 1), sizeof(unsigned long long));
        config->size_T = config->max_size - (config->min_size - 1);
    } else if (config->size_T < (config->max_size - (config->min_size - 1))) {
        config->T = (unsigned long long *) realloc(config->T, (config->max_size - (config->min_size - 1)) * sizeof(unsigned long long));
        config->size_T = (config->max_size - (config->min_size - 1));
    }
    unsigned long long length =  (unsigned long long) config->alphabet_size;
    unsigned long long power = length;
    for (size_t i = 1; i < config->min_size - 1; i++) {
        power *= length;
    }
    for (size_t i = config->min_size; i <= config->max_size; i++) {
        power *= length;
        config->T[i - config->min_size] = power;
        config->N += config->T[i - config->min_size];
    }
}

void freeConfig(struct Config* config)
{
    if (config == NULL) {
        return;
    }
    if (config->T) {
        free(config->T);
    }
    free(config);
}