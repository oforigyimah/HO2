#include "../helper_func/helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "child_ohno.h"
#include "../../external/hash/include/hash.h"

int main(int argc, char *argv[]){
    unsigned int len;
    char num[512];
    long unsigned int start, end, j;
    int bool;
    uint8_t sha256_byte_array[SHA256_HASH_SIZE];
    uint8_t sha512_byte_array[SHA512_HASH_SIZE];
    char sha256_hex_string[SHA256_STRING_HASH_SIZE];
    char sha512_hex_string[SHA512_STRING_HASH_SIZE];
    char starting_hash[SHA512_STRING_HASH_SIZE];
    char sliced_hex_string[SHA256_STRING_HASH_SIZE];
    sha2_context ctx;

    if (argc != 3){
        printf("Usage: %s <start> <end>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    start = strtoul(argv[1], NULL, 10);
    end = strtoul(argv[2], NULL, 10);

    for (j = start; j < end; j++) {
        for (int q = 0; q < 2; q++) {
            if (q) sprintf(num, "%ld", j);
            else sprintf(num, "%ld", -j);
            printf("num is %s\n", num);
            sha512(num, 512, sha512_byte_array);
            byte_array_to_hex(sha512_byte_array, sha512_hex_string, SHA512_HASH_SIZE);

            for (int a = 0; a < 64; a++) {
                slice(sha512_hex_string, sliced_hex_string, a, 64 + a);
                strncpy(starting_hash, sliced_hex_string, SHA512_STRING_HASH_SIZE);

                for (int b = 0; b < ITERATIONS; b++) {
                    polarssl_sha256(&ctx, (unsigned char *)starting_hash, 128, sha256_byte_array);
                    byte_array_to_hex(sha256_byte_array, sha256_hex_string, SHA256_HASH_SIZE);
                    strncpy(starting_hash, sha256_hex_string, SHA256_STRING_HASH_SIZE);
                }
            }
        }
    }
    return 0;
}