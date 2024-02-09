#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include "helper.h"

#ifdef _WIN32
    #include <direct.h>
    #include <io.h>
    #include <sys/stat.h> // Include this
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
        return 0;
    }
    return 1;
}

int file_exists(const char *filepath) {
    if (access(filepath, 0) != -1) {
        // file exists
        return 1;
    } else {
        // file doesn't exist
        return 0;
    }
}

int create_app_dir(){
    char *homeDir = get_home_dir();

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




