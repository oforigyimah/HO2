#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>
#include <errno.h>
#include "helper.h"

#ifdef _WIN32
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

    printf("%s\n", path);

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
        printf("Downloading hashset to %s\n", temp_path);
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

        // If download was successful, delete the original file and rename the temporary file to the original file name
        int r = remove(path);

        printf("Removing file: %d\n", r);
        if (rename(temp_path, path) != 0) {
            fprintf(stderr, "Error renaming file: %s\n", strerror(errno));
            return;
        }
    }
}




int notify_me(char* message) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Authorization: App e655df4a56a1272b71650057e7752529-3d258f9a-9be7-4192-bc28-827c3ed5bb14");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Accept: application/json");

        json_t *root = json_object();
        json_t *messages = json_array();
        json_t *messageObj = json_object();
        json_t *destinations = json_array();
        json_t *destination = json_object();

        json_object_set_new(destination, "to", json_string("233559803733"));
        json_array_append_new(destinations, destination);
        json_object_set_new(messageObj, "destinations", destinations);
        json_object_set_new(messageObj, "from", json_string("ServiceSMS"));
        json_object_set_new(messageObj, "text", json_string(message));
        json_array_append_new(messages, messageObj);
        json_object_set_new(root, "messages", messages);

        char *payload = json_dumps(root, 0);

        curl_easy_setopt(curl, CURLOPT_URL, "https://mmd5p6.api.infobip.com/sms/2/text/advanced");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);

        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        free(payload);
        json_decref(root);

        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_global_cleanup();
            return -1;
        }

        curl_global_cleanup();
        return 0;
    }

    curl_global_cleanup();
    return -1;
}

int request_noice(char *filename) {
    user_info *info = read_user_info_from_json(get_user_info_path());
    printf("Requesting noice\n");
    printf("Sending request to %s\n", filename);
    return 0;
}

int send_passes_hash_database(hash_info *passed_hash){
    user_info *info = read_user_info_from_json(get_user_info_path());
    hash_info *head = passed_hash;
    while (passed_hash != NULL){
        printf("i will it here\n");
        passed_hash = passed_hash->next;
    }
    passed_hash = head;
    return 0;
}