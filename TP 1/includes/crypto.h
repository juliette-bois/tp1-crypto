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

void hash_MD5(const char* s, byte* digest);
void hash_SHA1(const char* s, byte* digest);
void H(const char* s, byte* digest);
void displayDigest(int length, byte* digest);
char* i2c(int digest, struct Config* config);
uint64_t h2i(byte* digest, int t, struct Config* config);
uint64_t i2i(int digest, int i, struct Config* config);
uint64_t nouvelle_chaine(int idx1, size_t largeur, struct Config* config);
void freeCrypto();

#endif //TP_1_CRYPTO_H