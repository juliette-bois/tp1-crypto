//
// Created by Juliette on 16/10/2021.
//

#ifndef TP_1_CONFIG_H
#define TP_1_CONFIG_H

#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Config {
    char* alphabet;
    size_t alphabet_size;
    size_t min_size;
    size_t max_size;
    unsigned long long N;
    unsigned long long* T;
    size_t size_T;
};

struct Config* generateConfig(char* alphabet, size_t min_size, size_t max_size);
void computeN(struct Config* config);
void freeConfig(struct Config* config);

#endif //TP_1_CONFIG_H