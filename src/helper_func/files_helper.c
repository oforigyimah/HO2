#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <jansson.h>
#include <dirent.h>
#include "helper.h"

#ifdef _WIN32
    #include <io.h>
    #include <sys/stat.h>

#define mkdir(path, mode) _mkdir(path)
    #define stat _stat
    #define access _access
    struct _stat st;
    #define PATH_SEPARATOR '\\'
#else
    #include <sys/stat.h>
    #include <unistd.h>
    struct stat st;
    #define PATH_SEPARATOR '/'
#endif


#define MAX_HASHES 30

long unsigned int get_noice(char *filepath){
    if (file_exists(filepath) == -1){
        if (check_internet_connection() == 1){
            if (request_noice(get_noice_path()) == -1) {
                perror("Error requesting noice\n");
                perror("Exiting... try again later\n");
                printf("Press enter to exit: ");
                scanf("%s");
                exit(EXIT_FAILURE);
            }
        } else {
            perror("Internet connection require to get noice\n");
            perror("Please connect to internet and try again\n");
            printf("Press Enter to Exit: ");
            scanf("%s");
            exit(EXIT_FAILURE);

        }
    }
    FILE *fp;
    long unsigned int noice = -1;
    fp = fopen(filepath, "r");
    if (fp != NULL){
        fscanf(fp, "%ld", &noice);
        fclose(fp);
    }
    return noice;
}

void update_noice(char *filepath, int num){
    long unsigned int prev_noice = get_noice(filepath);
    if(prev_noice != -1){
        FILE *fp;
        fp = fopen(filepath, "w");
        if (fp != NULL){
            fprintf(fp, "%ld", prev_noice + num);
            fclose(fp);
        }
    }
}

int get_hash(char *filepath, char *hashes[MAX_HASHES], char *games[MAX_HASHES]){
    FILE *fp_hashes;
    char buffer[1024];
    char *record, *line;
    int len = 0;

    if (file_exists(filepath) == -1){
        if (check_internet_connection() == 1){
            download_hashset(filepath);
        } else {
            prompt_hashset_missing();
        }
    }

    fp_hashes = fopen(filepath,"r");

    if(fp_hashes != NULL) {
        while((line=fgets(buffer,sizeof(buffer),fp_hashes))!=NULL && len < MAX_HASHES) {
            record = strtok(line,",");
            if(record != NULL) {
                free(hashes[len]); // Free the previously allocated memory
                hashes[len] = strdup(record);
                record = strtok(NULL,",");
                if(record != NULL) {
                    strncpy(games[len], record, strlen(record));
                }
                len++;
            }
        }
        fclose(fp_hashes);
    }
    return len;
}


int handle_passed_hash(char *passed_hash,char *hash_str ,char *path, int index){
    // Check if the directory exists
    printf("+++++++++++++++++++++++++++++++++++++++++++++++");
    if (stat(path, &st) == -1) {
        // If the directory does not exist, create it
        if (mkdir(path, 0700) == -1) {
            perror("Error creating directory");
            return 1;
        }
    }

    time_t  now = time(NULL);
    char filename[128];
    sprintf(filename, "%s/%ld", path, now);
    FILE *fp = fopen(filename, "w");
    if (fp != NULL) {
        fprintf(fp, "%s\t%s\t%d", passed_hash, hash_str, index);
        fclose(fp);
        printf("passed_hash: %s\n", passed_hash);

        if(check_internet_connection() == 0){
            char message[512];
            found_hash_info *info = malloc(sizeof(found_hash_info));
            info->user_id = get_user_info()->user_id;
            info->found_hash = passed_hash;
            sprintf(info->game, "%s", index);
            send_passes_hash_database(info);
            sprintf(message, "%s\t%s\t%d", passed_hash, hash_str, index);
            notify_me(message);
        }

        return 0;
    }
    return 1;
}

int file_exists(const char *filepath) {
    if (access(filepath, 0) != -1) {
        // file exists
        return 0;
    } else {
        // file doesn't exist
        return -1;
    }
}

int create_app_dir(){

    char *appDir = get_app_dir();

    if (stat(appDir, &st) == -1)
        if (mkdir(appDir, 0700) == -1) {
            perror("create app dir failed");
            return -1;
        }
    return 0;
}

int create_hash_dir(){
    char *appDir = get_app_dir();
    if (stat(appDir, &st) == -1){
        if (create_app_dir() == -1){
            perror("create app dir failed");
            return -1;
        }
    }
    char hashDir[256];
    sprintf(hashDir, "%s/%s",appDir, ".hash");

    if (stat(hashDir, &st) == -1)
        if (mkdir(hashDir, 0700) == -1) {
            perror("create hash dir failed");
            return -1;
        }
    return 0;
}
int create_passed_hash_dir(){
    char *appDir = get_app_dir();
    if (stat(appDir, &st) == -1){
        if (create_app_dir() == -1){
            perror("create app dir failed");
            return -1;
        }
    }
    char passedHashDir[256];
    sprintf(passedHashDir, "%s/%s",appDir, ".passed_hash");

    if (stat(passedHashDir, &st) == -1)
        if (mkdir(passedHashDir, 0700) == -1) {
            perror("create passed hash dir failed");
            return -1;
        }
    return 0;
}

int create_noice_dir(){
    char *appDir = get_app_dir();
    if (stat(appDir, &st) == -1){
        if (create_app_dir() == -1){
            perror("create app dir failed");
            return -1;
        }
    }
    char noiceDir[256];
    sprintf(noiceDir, "%s/%s",appDir, ".noice");

    if (stat(noiceDir, &st) == -1)
        if (mkdir(noiceDir, 0700) == -1) {
            perror("create noice dir failed");
            return -1;
        }
    return 0;
}



int store_file_paths(const char *dir_path, hash_info **head) {
    struct dirent *entry;
    DIR *dir = opendir(dir_path);
    hash_info *current = *head;
    int index = 0;
    int file_count = 0; // Add a counter for the number of files

    if (dir == NULL) {
        printf("Error opening directory '%s'\n", dir_path);
        return -1;
    }

    // If the list already exists, traverse to the end
    if (current != NULL) {
        while (current->next != NULL) {
            current = current->next;
            index++;
        }
    }

    while ((entry = readdir(dir)) != NULL) {
        struct stat path_stat;
        char full_path[1024];
        sprintf(full_path, "%s%c%s", dir_path, PATH_SEPARATOR, entry->d_name);
        stat(full_path, &path_stat);
        if (S_ISREG(path_stat.st_mode)) { // Check if the entry is a regular file
            hash_info *new_node = (hash_info*)malloc(sizeof(hash_info));
            new_node->path = (char*)malloc(strlen(full_path) + 1);
            strcpy(new_node->path, full_path);
            new_node->next = NULL;
            new_node->index = index++;

            if (current == NULL) {
                *head = new_node;
                current = *head;
            } else {
                current->next = new_node;
                current = new_node;
            }
            file_count++; // Increment the counter when a file is found
        }
    }

    closedir(dir);

    if (file_count == 0) {
        return -1; // Return -1 if no files were found
    }

    return 0; // Return 0 if files were found
}


void retrieve_elements(const char* path, char** passed_hash, char** hash, char** game) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[256];
    if (fgets(line, sizeof(line), file) != NULL) {
        char* token = strtok(line, "\t");
        if (token != NULL) {
            *passed_hash = strdup(token);
            token = strtok(NULL, "\t");
        }
        if (token != NULL) {
            *hash = strdup(token);
            token = strtok(NULL, "\t");
        }
        if (token != NULL) {
            *game = strdup(token);
        }
    }

    fclose(file);
}

void init (){
    if (file_exists(get_hash_path()) == 0)
        return;
    if (file_exists(get_user_info_path()) == 0)
        return;
    create_app_dir();
    create_passed_hash_dir();
    create_hash_dir();
    create_noice_dir();
    user_info *info = get_user_info();
    write_user_info_to_json(info, get_user_info_path());
    if (check_internet_connection() == 1){
        download_hashset(get_hash_path());
        if (request_noice(get_noice_path()) == -1){
            prompt_hashset_missing();
        }
    }
    else {
        printf("Please check your internet connection\n");
        printf("You will need an internet connection to download the hashset\n");
        printf("This program will not work without the hashset\n");
        printf("Exiting... check your internet connection and rerun the program\n");
        exit(EXIT_FAILURE);
    }
    printf("Initialization complete\n");
    clear_terminal();
}

void write_user_info_to_json(user_info *info, const char *filename) {
    json_t *root = json_object();
    json_object_set_new(root, "name", json_string(info->name));
    json_object_set_new(root, "secret", json_string(info->secret));
    json_object_set_new(root, "email", json_string(info->email));
    json_object_set_new(root, "phone", json_string(info->phone));
    json_object_set_new(root, "pc_name", json_string(info->pc_name));
    json_object_set_new(root, "user_id", json_string(info->user_id));

    json_dump_file(root, filename, JSON_INDENT(4));
    printf("User info written to %s\n", filename);
    json_decref(root);
}

user_info* read_user_info_from_json(const char *filename) {
    json_error_t error;
    json_t *root = json_load_file(filename, 0, &error);
    if (!root) {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        return NULL;
    }

    user_info *info = malloc(sizeof(user_info));
    info->name = strdup(json_string_value(json_object_get(root, "name")));
    info->secret = strdup(json_string_value(json_object_get(root, "secret")));
    info->email = strdup(json_string_value(json_object_get(root, "email")));
    info->phone = strdup(json_string_value(json_object_get(root, "phone")));
    info->pc_name = strdup(json_string_value(json_object_get(root, "pc_name")));
    info->user_id = strdup(json_string_value(json_object_get(root, "user_id")));

    json_decref(root);
    return info;
}

