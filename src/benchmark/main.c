#include <stdio.h>
#include <string.h>
#include <time.h>
#include <openssl/evp.h>
#include "../../external/hash/include/hash.h"

#define ITERATIONS 100000000


void openssl_sha256(unsigned char *input, size_t length, unsigned char *hash, const EVP_MD *md, EVP_MD_CTX *mdctx, unsigned int len) {
    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, input, length);
    EVP_DigestFinal_ex(mdctx, hash, &len);
    EVP_MD_CTX_free(mdctx);
}

void hash_to_hex(unsigned char *hash, char *hexstr, unsigned int len) {
    for(unsigned int i = 0; i < len; i++) {
        sprintf(hexstr + (i * 2), "%02x", hash[i]);
    }
}

int main() {
    unsigned char hash[EVP_MAX_MD_SIZE];
    char hexstr[(EVP_MAX_MD_SIZE * 2) + 1];
    char *input = "Hello, World! my name is sha2!";
    clock_t start, end;
    double cpu_time_used;
    unsigned int len;

    start = clock();
    for(int j = 0; j < ITERATIONS; j++) {


    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by PolarSSL SHA-256: %f\n", cpu_time_used);

    start = clock();
    for(int j = 0; j < ITERATIONS; j++) {
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by PolarSSL as a function SHA-256: %f\n", cpu_time_used);

    start = clock();
    for(int j = 0; j < ITERATIONS; j++) {
        sha256(input, strlen(input), hash);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by custom SHA-256: %f\n", cpu_time_used);

    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    OpenSSL_add_all_digests();
    md = EVP_get_digestbyname("SHA256");

    mdctx = EVP_MD_CTX_new();

    start = clock();
    for(int j = 0; j < ITERATIONS; j++) {
        EVP_DigestInit_ex(mdctx, md, NULL);
        EVP_DigestUpdate(mdctx, input, strlen(input));
        EVP_DigestFinal_ex(mdctx, hash, &len);
    }
    EVP_MD_CTX_free(mdctx);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by OpenSSL SHA-256: %f\n", cpu_time_used);

    start = clock();
    for(int j = 0; j < ITERATIONS; j++) {
        openssl_sha256((unsigned char *)input, strlen(input), hash, md, mdctx, len);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by OpenSSL as a function SHA-256: %f\n", cpu_time_used);

    return 0;
}