
#include <stdint.h>

#define SHA1_HASH_SIZE 20
#define SHA1_STRING_HASH_SIZE ((SHA1_HASH_SIZE * 2) + 1)

#define SHA224_HASH_SIZE 28
#define SHA224_STRING_HASH_SIZE ((SHA224_HASH_SIZE * 2) + 1)

#define SHA256_HASH_SIZE 32
#define SHA256_STRING_HASH_SIZE ((SHA256_HASH_SIZE * 2) + 1)

#define SHA384_HASH_SIZE 48
#define SHA384_STRING_HASH_SIZE ((SHA384_HASH_SIZE * 2) + 1)

#define SHA512_HASH_SIZE 64
#define SHA512_STRING_HASH_SIZE ((SHA512_HASH_SIZE * 2) + 1)

#define SHA512_224_HASH_SIZE 28
#define SHA512_224_STRING_HASH_SIZE ((SHA512_224_HASH_SIZE * 2) + 1)

#define SHA512_256_HASH_SIZE 32
#define SHA512_256_STRING_HASH_SIZE ((SHA512_256_HASH_SIZE * 2) + 1)

void sha1(const void * const data, const size_t size, uint8_t hash[SHA1_HASH_SIZE]);

void sha1_hash_to_string(const uint8_t hash[SHA1_HASH_SIZE], char dest[SHA1_STRING_HASH_SIZE]);



void sha224(const void * const data, const size_t size, uint8_t hash[SHA224_HASH_SIZE]);

void sha224_hash_to_string(const uint8_t hash[SHA224_HASH_SIZE], char dest[SHA224_STRING_HASH_SIZE]);


void sha256(const void * const data, const size_t size, uint8_t hash[SHA256_HASH_SIZE]);

void sha256_hash_to_string(const uint8_t hash[SHA256_HASH_SIZE], char dest[SHA256_STRING_HASH_SIZE]);


void sha384(const void * const data, const size_t size, uint8_t hash[SHA384_HASH_SIZE]);

void sha384_hash_to_string(const uint8_t hash[SHA384_HASH_SIZE], char dest[SHA384_STRING_HASH_SIZE]);


void sha512(const void * const data, const size_t size, uint8_t hash[SHA512_HASH_SIZE]);

void sha512_hash_to_string(const uint8_t hash[SHA512_HASH_SIZE], char dest[SHA512_STRING_HASH_SIZE]);


void sha512_224(const void * const data, const size_t size, uint8_t hash[SHA512_224_HASH_SIZE]);

void sha512_224_hash_to_string(const uint8_t hash[SHA512_224_HASH_SIZE], char dest[SHA512_224_STRING_HASH_SIZE]);


void sha512_256(const void * const data, const size_t size, uint8_t hash[SHA512_256_HASH_SIZE]);

void sha512_256_hash_to_string(const uint8_t hash[SHA512_256_HASH_SIZE], char dest[SHA512_256_STRING_HASH_SIZE]);