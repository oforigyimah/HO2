#include <stdio.h>
#include <curl/curl.h>

int main() {
    printf("Hello, World!\n");

    curl_version_info_data* data = curl_version_info(CURLVERSION_NOW);
    printf("Curl version: %s\n", data->version);

    return 0;
}
