#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <errno.h>

#ifdef _WIN32
    #include <direct.h>
    #define mkdir(path, mode) _mkdir(path)
    #define stat _stat
#else
    #include <sys/stat.h>
#endif

#define FILE_URL "https://storage.googleapis.com/simplesmsmessenger.appspot.com/hash.csv"

void download_file(char *path) {
    CURL *curl;
    FILE *fp;
    int result;
    char *filename = "hash.csv";
    char fullpath[512];

    // Check if the directory exists
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        // If the directory does not exist, create it
        if (mkdir(path, 0700) == -1) {
            perror("Error creating directory");
            return;
        }
    }

    snprintf(fullpath, sizeof(fullpath), "%s%s", path, filename); // concatenate path and filename

    curl = curl_easy_init();
    if (curl) {
        fp = fopen(fullpath,"wb");
        if(fp == NULL) {
            fprintf(stderr, "Error opening file: %s\n", strerror(errno));
            return;
        }
        curl_easy_setopt(curl, CURLOPT_URL, FILE_URL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        result = curl_easy_perform(curl);
        if(result != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));

        curl_easy_cleanup(curl);
        fclose(fp);
    }
}