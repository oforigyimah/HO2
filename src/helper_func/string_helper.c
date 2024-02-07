#include <stdint.h>

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
        uint8_t b1 = (b >> 4) & 0x0f;
        uint8_t b2 = b & 0x0f;

        hex_string[idx_ * 2] = (b1 < 10) ? (b1 + '0') : (b1 - 10 + 'a');
        hex_string[idx_ * 2 + 1] = (b2 < 10) ? (b2 + '0') : (b2 - 10 + 'a');
    }
    hex_string[byte_array_length * 2] = '\0';
}




void slice(const char* str, char* buffer, int start, int end) {
    int j = 0;
    for (int i = start; i < end; i++) {
        buffer[j++] = str[i];
    }
    buffer[j] = '\0';
}

void reverse(char *str, int length)
{
    int start;
    int end = length -1;
    for(start = 0; start < end; ++start, --end)
    {
        const char ch = str[start];
        str[start] = str[end];
        str[end] = ch;
    }
}
