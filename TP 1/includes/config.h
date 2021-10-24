//
// Created by Juliette on 16/10/2021.
//

#ifndef TP_1_CONFIG_H
#define TP_1_CONFIG_H

#include <stdlib.h>
#include <string.h>
#include <math.h>

enum algorithm {
    A_MD5 = 0,
    A_SHA1 = 1
};

struct Config {
    char* alphabet;
    size_t alphabet_size;
    size_t min_size;
    size_t max_size;
    uint64_t N;
    uint64_t* T;
    size_t size_T;
    enum algorithm algorithm;
};

struct Config* generateConfig(char* alphabet, size_t min_size, size_t max_size, enum algorithm algorithm);
void computeN(struct Config* config);
void freeConfig(struct Config** config);
void displayConfig(struct Config* config);

#endif //TP_1_CONFIG_H