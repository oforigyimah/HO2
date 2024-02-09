#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <errno.h>
#include "helper.h"

#ifdef _WIN32
    #include <direct.h>
     #include <sys/stat.h>
    #define mkdir(path, mode) _mkdir(path)
    #define stat _stat
#else
    #include <sys/stat.h>
#endif

#define FILE_URL "https://storage.googleapis.com/simplesmsmessenger.appspot.com/hash.csv"



int check_internet_connection() {
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.com");
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            return 0;
        }
        /* always cleanup */
        curl_easy_cleanup(curl);
        return 1;
    }
    return 0;
}


void download_hashset(char *path) {
    CURL *curl;
    FILE *fp;
    int result;
    char temp_path[256]; // buffer for temporary file path

    if (!check_internet_connection()) {
        fprintf(stderr, "No internet connection\n");
        return;
    }
    // Check if the directory exists
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        // If the directory does not exist, create it
        if (create_hash_dir() == -1) {
            perror("Error creating directory");
            return;
        }
    }

    curl = curl_easy_init();
    if (curl) {
        snprintf(temp_path, sizeof(temp_path), "%s.temp", path); // create temporary file path
        fp = fopen(temp_path,"wb");
        if(fp == NULL) {
            fprintf(stderr, "Error opening file: %s\n", strerror(errno));
            return;
        }
        curl_easy_setopt(curl, CURLOPT_URL, FILE_URL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        result = curl_easy_perform(curl);
        if(result != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
            curl_easy_cleanup(curl);
            fclose(fp);
            remove(temp_path); // remove the temporary file if download failed

            return;
        }

        curl_easy_cleanup(curl);
        fclose(fp);

        // If download was successful, rename the temporary file to the original file name
        if (rename(temp_path, path) != 0) {
            fprintf(stderr, "Error renaming file: %s\n", strerror(errno));
            return;
        }
    }
}