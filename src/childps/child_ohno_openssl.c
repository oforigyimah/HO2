#include "../helper_func/helper.h"
#include "child_ohno.h"
#include "../../external/hash/include/hash.h"
#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char *argv[]){
#ifdef _WIN32
    SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED);
#endif

    if (argc != 2){
        printf("Usage: %s <noice>\n", argv[0]);
        exit(EXIT_FAILURE);
    }


    long unsigned int noice = strtoul(argv[1], NULL, 10);
    char noice_str[512];

    uint8_t sha512_byte_array[SHA512_HASH_SIZE];
    char sha512_hex_string[SHA512_STRING_HASH_SIZE];
    char starting_hash[SHA256_STRING_HASH_SIZE];
    char sliced_hex_string[SHA256_STRING_HASH_SIZE];

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    EVP_MD_CTX *mdctx;
    OpenSSL_add_all_digests();
    mdctx = EVP_MD_CTX_create();

    clock_t start, end;
    double cpu_time_used;

    int hash_count;
    char *hashes_path = get_hash_path();
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

    start = clock();

    for (int q= 0; q < 2; q++){
        if (q) sprintf(noice_str, "%ld", noice);
        else sprintf(noice_str, "-%ld", noice);
        sha512(noice_str, strlen(noice_str), sha512_byte_array);
        byte_array_to_hex(sha512_byte_array, sha512_hex_string, SHA512_HASH_SIZE);
        for (int w = 0; w < 2; w++) {
            if (w) reverse(sha512_hex_string, SHA512_STRING_HASH_SIZE -1);
            for (int e = 0; e < 65; e++){
                slice(sha512_hex_string, sliced_hex_string, e, 64 + e);
                strcpy(starting_hash, sliced_hex_string);
                for (int r = 0; r < ITERATIONS; r++){
                    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
                    EVP_DigestUpdate(mdctx, starting_hash, 64);
                    EVP_DigestFinal_ex(mdctx, hash, &hash_len);
                    byte_array_to_hex(hash, starting_hash, SHA256_HASH_SIZE);
                    if (compare(sliced_hex_string, starting_hash, (const char **) hashes, hash_count) == 0) {
                        printf("\n\n\n\n\n\nFound match for %s: %s\n", sliced_hex_string, starting_hash);
                        break;
                    }
                }
            }
        }
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken of noice: %ld %fs second\n",noice, cpu_time_used);

    EVP_MD_CTX_destroy(mdctx);
    OPENSSL_cleanup();
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

    printf("Noice: %ld has finished\n", noice);
    return 0;


}