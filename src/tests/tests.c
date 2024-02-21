#include "../helper_func/helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){
    char *home = get_home_dir();
    printf("Home: %s\n", home);

    printf("Testing for download_hashset\n");
    char *path = get_hash_path();
    printf("Path: %s\n", path);

    download_hashset(path);
    if (file_exists(path)){
        printf("File exists\n");
    } else {
        printf("File does not exist\n");
    }

    request_noice("hello");

    found_hash_info *info = (found_hash_info*)malloc(sizeof(found_hash_info));
    info->found_hash = "hello";
    info->game = "21";
    info->user_id = "1";
    printf("Begin testing for send_passes_hash_database\n");
    send_passes_hash_database(info);
    printf("Testing for send_passes_hash_database\n");
    free(info);
    info = NULL;

    return 0;

}