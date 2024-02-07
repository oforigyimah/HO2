#ifndef CHILD_OHNO_H
#define CHILD_OHNO_H

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../../external/polarssl/include/polarssl/sha2.h"
#include "../helper_func/helper.h"

#ifdef _WIN32
//    #include "C:\\Program Files\\OpenSSL-Win64\\include\\openssl\\evp.h"
#include <openssl/evp.h>

#else
    #include <openssl/evp.h>
#endif

#define ITERATIONS 50

void polarssl_sha256(sha2_context *ctx, unsigned char *input, size_t length, unsigned char *hash) {
    sha2_starts(ctx, 0); // 0 for SHA-256, 1 for SHA-224
    sha2_update(ctx, input, length);
    sha2_finish(ctx, hash);
}

void openssl_sha256(char *input, size_t length, unsigned char *hash, const EVP_MD *md, EVP_MD_CTX *mdctx, unsigned int *len) {
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, input, length);
    EVP_DigestFinal_ex(mdctx, hash, len);
}

void hash_to_hex(unsigned char *hash, char *hexstr, unsigned int len) {
    for(unsigned int i = 0; i < len; i++) {
        sprintf(hexstr + (i * 2), "%02x", hash[i]);
    }
}

void compare(char* hexstr, char **hashes, int hashes_len){
    int bool;
    for (int i = 0; i < hashes_len; i++){
        bool = strcmp(hexstr, hashes[i]);
        if (bool != 0) continue;
        else {
            handle_passed_hash(hexstr);
            return;
        }
    }
}




#endif // CHILD_OHNO_H