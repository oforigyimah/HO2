#include "helper_func/helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char *home = get_home_dir();
    printf("Home: %s\n", home);
    return 0;
}