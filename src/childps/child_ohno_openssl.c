#include "child_ohno.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../external/hash/include/hash.h"
#include "../helper_func/helper.h"



int main(int argc, char *argv[]) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    char hexstr[(EVP_MAX_MD_SIZE * 2) + 1];
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

    if (argc != 3){
        printf("Usage: %s <start> <end>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    start = strtoul(argv[1], NULL, 10);
    end = strtoul(argv[2], NULL, 10);

    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    OpenSSL_add_all_digests();
    md = EVP_get_digestbyname("SHA256");

    for (j = start; j < end; j++){
        for (int q = 0; q < 2; q++){
            if (q) sprintf(num, "%ld", j);
            else sprintf(num, "%ld", -j);
            printf("num is %s\n", num);
            sha512(num, strlen(num), sha512_byte_array);
            byte_array_to_hex(sha512_byte_array, sha512_hex_string, SHA512_HASH_SIZE);

            for (int a = 0; a < 64; a++){
                slice(sha512_hex_string, sliced_hex_string, a, 64 + a);
                strncpy(starting_hash, sliced_hex_string, SHA512_STRING_HASH_SIZE);

                for (int b = 0; b  < ITERATIONS; b ++){
                    mdctx = EVP_MD_CTX_new();
                    EVP_DigestInit_ex(mdctx, md, NULL);
                    EVP_DigestUpdate(mdctx, starting_hash, strlen(starting_hash));
                    EVP_DigestFinal_ex(mdctx, hash, &len);
                    EVP_MD_CTX_free(mdctx);
                    hash_to_hex(hash, hexstr, len);
                    strncpy(starting_hash, hexstr, SHA512_STRING_HASH_SIZE);
                    printf("%s\n", hexstr);
                }
            }



        }
    }

}
