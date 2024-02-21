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
#define BASE_URL "https://amo2.000webhostapp.com/"
#define LOGIN "oforilogin.php"
#define CONNECT "oforiconnect.php"
#define HASH_STORE "oforihashstore.php"
#define GET "oforiget.php"


void write_to_file(char *path, char *str);

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

    char *url = malloc(512 * sizeof(char));
    sprintf(url, "%s%s?name=%s&secret=%s&email=%s&phonenumber=%s&pcname=%s&userid=%s", BASE_URL, LOGIN, info->name, info->secret, info->email, info->phone, info->pc_name, info->user_id);
    replace_plus_with_space(url);
    printf("Requesting noice from %s\n", url);

    CURL *curl = curl_easy_init();
    struct string s;
    init_string(&s);

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        CURLcode res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            return -1;
        } else {
            json_error_t error;
            json_t *root = json_loads(s.ptr, 0, &error);
            printf("Response: %s\n", s.ptr);

            if(!root) {
                fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
                return -1;
            }

            json_t *startnum = json_object_get(root, "startnum");
            if(!json_is_string(startnum)) {
                fprintf(stderr, "error: commit %s is not a string\n", "startnum");
                json_decref(root);
                return -1;
            }

            noice_info info_n;
            info_n.start = atoi(json_string_value(startnum));
            info_n.end = info_n.start + 1000000; // Set end and status as per your requirement
            info_n.status = 0;

            char *start = malloc(512 * sizeof(char));
            sprintf(start, "%d", info_n.start);
            write_to_file(get_noice_path(), start);
            free(start);

            char *end = malloc(512 * sizeof(char));
            sprintf(end, "%d", info_n.end);
            sprintf(filename, "%s.end", filename);
            write_to_file(filename, end);
            free(end);





            printf("Noice: %d\n", info_n.start);

            json_decref(root);
            printf("after json_decref\n");
        }

    }
    printf("Requesting noice from %s complete\n", url);
    free(s.ptr);
    free(url);

    return 0;
}

void write_to_file(char *path, char *str) {
    FILE *fp = fopen(path, "w");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }
    fprintf(fp, "%s", str);
    fclose(fp);
}

int send_passes_hash_database(found_hash_info *info) {
    char *url = malloc(512 * sizeof(char));
    sprintf(url, "%s%s?userid=%s&foundhash=%s&game=%s", BASE_URL, HASH_STORE, info->user_id, info->found_hash, info->game);
    replace_plus_with_space(url);
    printf("Sending found hash to %s\n", url);

    CURL *curl = curl_easy_init();
    struct string s;
    init_string(&s);

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        CURLcode res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        } else {
            json_error_t error;
            json_t *root = json_loads(s.ptr, 0, &error);
            printf("Response: %s\n", s.ptr);

            if(!root) {
                fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
                return 1;
            }

            json_t *result = json_object_get(root, "result");
            if(!json_is_string(result)) {
                fprintf(stderr, "error: commit %s is not a string\n", "result");
                json_decref(root);
                return 1;
            }

            printf("Status: %s\n", json_string_value(result));

            json_decref(root);
        }

        curl_easy_cleanup(curl);
    }
    return 0;
}