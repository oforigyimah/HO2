#include "helper_func/helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){
#ifdef _WIN32
    char program[] = "childps\\child_ohno_openssl.exe";
#else
    char program[] = "childps/child_ohno_openssl";
#endif
    char *args[] = {program, "0", "1", NULL};
    pid_t pid;
    int status;

    spawn_process(program, args, &pid);
    if (pid == -1) {
        perror("Process creation failed");
        exit(EXIT_FAILURE);
    }
    wait_for_process(pid, &status);
    if (WIFEXITED(status)) {
        printf("Child process exited with status %d\n", WEXITSTATUS(status));
    } else {
        printf("Child process did not exit normally\n");
    }
}