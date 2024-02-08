#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

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


int handle_passed_hash(char *passed_hash, char *path){
    struct stat st = {0};

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
        fprintf(fp, "%s", passed_hash);
        fclose(fp);
        memset(passed_hash, 0, sizeof(char) * 65);
        printf("passed_hash: %s\n", passed_hash);
        return 0;
    }
    return 1;
}