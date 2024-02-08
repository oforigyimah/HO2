#include "child_ohno.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../external/hash/include/hash.h"



int main(int argc, char *argv[]) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    char hexstr[129];
    unsigned int len;
    char num[512];
    long unsigned int number;

    uint8_t sha512_byte_array[SHA512_HASH_SIZE];
    char sha512_hex_string[SHA512_STRING_HASH_SIZE];
    char starting_hash[SHA512_STRING_HASH_SIZE];
    char sliced_hex_string[SHA256_STRING_HASH_SIZE];

    int hash_count;

    char *hashes_path = get_home_dir();

    char **hashes;
    char **games;

    hashes = malloc(MAX_HASHES * sizeof(char *));
    games = malloc(MAX_HASHES * sizeof(char *));


    if (hashes == NULL) {
        printf("Failed to allocate memory for hashes.\n");
        return 1;
    }
    if (games == NULL) {
        printf("Failed to allocate memory for games.\n");
        return 1;
    }

    for (int i = 0; i < MAX_HASHES; i++) {
        hashes[i] = malloc(65 * sizeof(char));
        memset(hashes[i], '\0', sizeof(char) * 65);
        if (hashes[i] == NULL) {
            printf("Failed to allocate memory for hashes[%d].\n", i);
            exit(1);
        }
    }

    for (int i = 0; i < MAX_HASHES; i++) {
        games[i] = malloc(33 * sizeof(char));
        memset(games[i], '\0', sizeof(char) * 33);
        if (games[i] == NULL) {
            printf("Failed to allocate memory for games[%d].\n", i);
            exit(1);
        }
    }

    hash_count = get_hash(hashes_path, hashes, games);


    if (argc != 3){
        printf("Usage: %s <start> <end>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    number = strtoul(argv[1], NULL, 10);

    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    OpenSSL_add_all_digests();
    md = EVP_get_digestbyname("SHA256");

    mdctx = EVP_MD_CTX_new();

        for (int q = 0; q < 2; q++){
            if (q) sprintf(num, "%ld", number);
            else sprintf(num, "%ld", -number);
            printf("num is %s\n", num);
            sha512(num, 512, sha512_byte_array);
            byte_array_to_hex(sha512_byte_array, sha512_hex_string, SHA512_HASH_SIZE);
            for (int w = 0; w < 2; w++) {
                if (w) reverse(sha512_hex_string, SHA512_STRING_HASH_SIZE);

                for (int a = 0; a < 64; a++) {
                    slice(sha512_hex_string, sliced_hex_string, a, 64 + a);
                    strncpy(starting_hash, sliced_hex_string, SHA512_STRING_HASH_SIZE);

                    for (int b = 0; b < ITERATIONS; b++) {
                        openssl_sha256(starting_hash, 128, hash, md, mdctx, &len);
                        byte_array_to_hex(hash, hexstr, len);
                        strncpy(starting_hash, hexstr, SHA512_STRING_HASH_SIZE);
                        if (compare(sliced_hex_string, hexstr, (const char **)hashes, hash_count) == 0) break;
                    }
                }
            }

        }

    EVP_MD_CTX_free(mdctx);
    for (int i = 0; i < MAX_HASHES; i++)
        if (hashes[i] != NULL) {
            free(hashes[i]);
            hashes[i] = NULL;
        }
    free(hashes);
    hashes = NULL;

    for (int i = 0; i < MAX_HASHES; i++)
        if (games[i] != NULL) {
            free(games[i]);
            games[i] = NULL;
        }
    free(games);
    games = NULL;

}
