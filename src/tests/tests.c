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

    return 0;

}