#include "../helper_func/helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){
    char *home = get_home_dir();
    printf("Home: %s\n", home);

    printf("Testing for download_hashset\n");
    char appdir[] = ".ho2";
    char *path = malloc(512);
    if (path == NULL){
        printf("failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    sprintf(path, "%s/%s", home, appdir);
    download_hashset("down");

    sprintf(path, "%s/%s", path, "hash.csv");
    if (file_exists("down/hash.csv")){
        printf("Download file test passed\n");
    } else {
        printf("Download file test failed\n");
    }

    free(path);
    return 0;

}