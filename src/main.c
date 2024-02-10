#include "helper_func/helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char *home = get_home_dir();
    printf("Home: %s\n", home);

    cpu_info cpu = get_cpu_info();
    printf("Cpu model is %s\n", cpu.model);
    printf("cpu cores is %d\n", cpu.cores);
    printf("cpu speed is %f\n", cpu.speed);

    return 0;
}