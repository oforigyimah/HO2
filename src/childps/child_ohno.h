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

#define ITERATIONS 2000000



void hash_to_hex(unsigned char *hash, char *hexstr, unsigned int len) {
    for(unsigned int i = 0; i < len; i++) {
        sprintf(hexstr + (i * 2), "%02x", hash[i]);
    }
}

int compare(char* parent_string,char* hexstr,const char **hashes, int hashes_len){
    for (int i = 0; i < hashes_len; i++){
        if (strcmp(hexstr, hashes[i]) != 0) continue;
        else {
            printf("Found match for %s: %s\n", parent_string, hexstr);
            handle_passed_hash(parent_string, hexstr,get_passed_hash_dir_path(), i);
            return 0;
        }
    }
    return -1;
}




#endif // CHILD_OHNO_H