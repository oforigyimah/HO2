#ifndef _HELPER_H
#define _HELPER_H

#include <stdint.h>

#define MAX_HASHES 30

long unsigned int get_noice(char *filepath);
void update_noice(char *filepath, int num);
int get_hash(char *filepath, char *hashes[MAX_HASHES], char *games[MAX_HASHES]);

int handle_passed_hash(char *passed_hash);

void hex_to_byte_array(const char* hex_sting, uint8_t* byte_array, int hex_string_length);
void byte_array_to_hex(const uint8_t* byte_array, char* hex_string, int byte_array_length);

void slice(const char* input, char* output, int start, int end);
void reverse(char* input, int length);

#endif // _HELPER_H