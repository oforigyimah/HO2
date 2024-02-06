#include <stdio.h>
#include <string.h>
#include <time.h>
#include <openssl/evp.h>
#include "../external/polarssl/include/polarssl/sha2.h"
#include "../external/hash/include/hash.h"

#define ITERATIONS 5000000

void polarssl_sha256(sha2_context *ctx, unsigned char *input, size_t length, unsigned char *hash) {
    sha2_starts(ctx, 0); // 0 for SHA-256, 1 for SHA-224
    sha2_update(ctx, input, length);
    sha2_finish(ctx, hash);
}

int main() {
    unsigned char hash[EVP_MAX_MD_SIZE];
    char *input = "Hello, World! my name is sha2!";
    sha2_context ctx;
    clock_t start, end;
    double cpu_time_used;
    unsigned int len;

    start = clock();
    for(int j = 0; j < ITERATIONS; j++) {
        sha2_starts(&ctx, 0); // 0 for SHA-256, 1 for SHA-224
        sha2_update(&ctx, (unsigned char *)input, strlen(input));
        sha2_finish(&ctx, hash);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by PolarSSL SHA-256: %f\n", cpu_time_used);

    start = clock();
    for(int j = 0; j < ITERATIONS; j++) {
        polarssl_sha256(&ctx, (unsigned char *)input, strlen(input), hash);
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

    start = clock();
    for(int j = 0; j < ITERATIONS; j++) {
        mdctx = EVP_MD_CTX_new();
        EVP_DigestInit_ex(mdctx, md, NULL);
        EVP_DigestUpdate(mdctx, input, strlen(input));
        EVP_DigestFinal_ex(mdctx, hash, &len);
        EVP_MD_CTX_free(mdctx);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken by OpenSSL SHA-256: %f\n", cpu_time_used);

    return 0;
}