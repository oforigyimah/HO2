#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_HASHES 30

long unsigned int get_noice(char *filepath){
    FILE *fp;
    long unsigned int noice;
    fp = fopen(filepath, "r");
    if (fp == NULL)
        return -1;
    fscanf(fp, "%ld", &noice);
    fclose(fp);
    return noice;
}

void update_noice(char *filepath, int num){
    FILE *fp;
    long unsigned int prev_noice = get_noice(filepath);
    fp = fopen(filepath, "w");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    fprintf(fp, "%ld", prev_noice + num);
    fclose(fp);
}

int get_hash(char *filepath, char *hashes[MAX_HASHES], char *games[MAX_HASHES]){
    FILE *fp_hashes;
    char buffer[1024];
    char *record, *line;
    int len = 0;

    fp_hashes = fopen(filepath,"r");

    if(fp_hashes == NULL) {
        printf("file opening failed to open %s\n", filepath);
        return -1 ;
    }
    while((line=fgets(buffer,sizeof(buffer),fp_hashes))!=NULL && len < MAX_HASHES) {
        record = strtok(line,",");
        if(record != NULL) {
            hashes[len] = strdup(record);
            record = strtok(NULL,",");
            if(record != NULL) {
                // Copy only non-null characters from record to games[len]
                strncpy(games[len], record, strlen(record));
            }
            len++;
        }
    }
    fclose(fp_hashes);
    return len;
}

int handle_passed_hash(char *passed_hash){
    time_t  now = time(NULL);
    char filename[20];
    sprintf(filename, "%ld", now);
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error Opening FIle\n");
        return 1;
    }

    fprintf(fp, "%s", passed_hash);
    fclose(fp);
    memset(passed_hash, 0, sizeof(char) * 65);
    printf("passed_hash: %s\n", passed_hash);
    return 0;

}



