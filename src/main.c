#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <polarssl/sha2.h>

int main() {
    unsigned char hash[32];
    char *input = "Hello, World!";
    sha2_context ctx;

    sha2_starts(&ctx, 0); // 0 for SHA-256, 1 for SHA-224
    sha2_update(&ctx, (unsigned char *)input, strlen(input));
    sha2_finish(&ctx, hash);

    // Print the hash in hexadecimal format
    for(int i = 0; i < sizeof(hash); i++) {
        printf("%02x", hash[i]);
    }

    printf("Hello, World!\n");

    curl_version_info_data* data = curl_version_info(CURLVERSION_NOW);
    printf("Curl version: %s\n", data->version);

    return 0;
}
