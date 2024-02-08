#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


static const char hex_lookup[] = "0123456789abcdef";

void hex_to_byte_array(const char* hex_string, uint8_t* byte_array, int hex_string_length) {
    for (int _idx = 0; _idx * 2 < hex_string_length; _idx++) {
        char c1 = hex_string[_idx * 2];
        char c2 = hex_string[_idx * 2 + 1];

        uint8_t b1 = (c1 <= '9') ? (c1 - '0') : (c1 - 'a' + 10);
        uint8_t b2 = (c2 <= '9') ? (c2 - '0') : (c2 - 'a' + 10);

        byte_array[_idx] = (b1 << 4) | b2;
    }
}

void byte_array_to_hex(const uint8_t* byte_array, char* hex_string, int byte_array_length) {
    for (int idx_ = 0; idx_ < byte_array_length; idx_++) {
        uint8_t b = byte_array[idx_];
        hex_string[idx_ * 2] = hex_lookup[b >> 4];
        hex_string[idx_ * 2 + 1] = hex_lookup[b & 0x0f];
    }
    hex_string[byte_array_length * 2] = '\0';
}

void slice(const char* str, char* buffer, int start, int end) {
    for (int i = start; i < end; i++) {
        buffer[i - start] = str[i];
    }
    buffer[end - start] = '\0';
}

void reverse(char *str, int length)
{
    char *start = str;
    char *end = str + length - 1;
    while (start < end)
    {
        char ch = *start;
        *start++ = *end;
        *end-- = ch;
    }
}

char* get_home_dir() {
    char* home_dir;

#ifdef _WIN32
    home_dir = getenv("APPDATA");
#else
    home_dir = getenv("HOME");
#endif

    if(home_dir == NULL) {
        fprintf(stderr, "Error: Could not find home directory.\n");
        return NULL;
    }

    return home_dir;
}