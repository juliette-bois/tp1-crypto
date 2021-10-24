//
// Created by Juliette on 17/10/2021.
//

#ifndef TP_1_CRYPTO_H
#define TP_1_CRYPTO_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <stdlib.h>
#include <config.h>

typedef unsigned char byte;

struct table {
    size_t width;
    size_t height;
    struct Config* config;
    size_t** table;
};

void hash_MD5(const char* s, byte* digest);
void hash_SHA1(const char* s, byte* digest);
void H(const char* s, byte* digest, struct Config* config);
int getDigestSize(struct Config* config);
void displayDigest(int length, byte* digest);
char* i2c(size_t digest, struct Config* config);
uint64_t h2i(byte* digest, size_t t, struct Config* config);
uint64_t i2i(size_t digest, size_t i, struct Config* config);
uint64_t new_chain(int idx1, size_t width, struct Config* config);
void freeCrypto();
int random_index();
struct table* create_table(size_t width, size_t height, struct Config* config);
void freeTable(struct table** table);
void saveTable(struct table* table, char* filename);
struct table* openTable(char* filename);
void displayTable(struct table* table, int displaytable);
int inverse(struct table* table, byte* digest, char* clear);
int search(struct table* table, uint64_t idx, int* a, int* b);
int verify_candidate(byte* digest, int t, int idx, char* clear, struct Config* config);
void displaySizeAndCoverage(const size_t width, const size_t height, uint64_t N);

#endif //TP_1_CRYPTO_H