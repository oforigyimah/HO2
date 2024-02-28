#include "helper_func/helper.h"
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define PRINT_LINE(TITLE) printf("\n\n************************ %s *************************\n\n", TITLE)


#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"
int main(){

#ifdef _WIN32
    int results = system("icacls .\\hashes /grant Everyone:(OI)(CI)F /T");
    if (results != 0){
        printf("Failed to grant permission to hashes directory\n");
        exit(EXIT_FAILURE);
    }
#endif

    init();
    PRINT_LINE("Welcome");
    char *welcome = "Welcome to HO2\nThis program is designed to  find the hash of a number\nand compare it with the hashset to find the number\nthat matches the hash.\n";
    char *notice = "This program needs hashset to work properly.\nPlease make sure that you update the hashset at least\nonce a day";
    char c;
    int cores_to_use;
    hash_info *h_info = NULL;
    int ret;

    printf("%s\n", welcome);
    printf("%s\n", notice);

    init();

    PRINT_LINE("Options");

    printf("Enter 1 to Update hashset\n");
    printf("Enter 2 to start computation\n");
    printf("Your choice: ");

    scanf("%c", &c);

    switch (c) {
        case '1':
            download_hashset(get_hash_path());
            printf("Hashset update\n");

        case '2':
            printf("Starting computation\n");
            break;
        default:
            printf("Invalid input\n");
            printf("starting computation\n");
    }


    PRINT_LINE("CPU Info");
    cpu_info cpu = get_cpu_info();
    printf("Cpu model is %s\n", &cpu.model);
    printf("cpu cores is %d\n", cpu.cores);
    printf("cpu clock speed is %f Mhz\n", cpu.speed);
    printf("It's recommended not to enter a number greater than the number of cores on your cpu\n");

   int result;
do {
    printf("Enter the number of cpu cores to use ==>  ");
    result = scanf("%d%*c", &cores_to_use);
    printf("\n");
    printf("You are using %d cores of your total %d cores\n", cores_to_use, cpu.cores);
    printf("Press Enter to continue: ");
    while(getchar() != '\n');
    if(result != 1) {
        printf("Invalid input, must be integer\n");
    }
} while(result != 1);
    clear_terminal();

    if (cores_to_use > cpu.cores){
        cores_to_use = cpu.cores;
        printf("Since the number of you want to use is greater than the available cores,");
        printf("for safety of your hardware the program will be using %d cores\n", cpu.cores);
    }

    pid_t pid_s[cores_to_use];
    int statuses[cores_to_use];
#ifdef _WIN32
    char *program = "ho2_child.exe";
#else
    char *program = "./ho2_child";
#endif


    char noice_str[50];
    long unsigned int noice;

    while (1){
        noice = get_noice(get_noice_path());

        if (noice == -1){
            printf("Failed to get noice\n");
            printf("Press Enter to continue: ");
            while(getchar() != '\n');
            exit(EXIT_FAILURE);
        }
        ret = store_file_paths(get_passed_hash_dir_path(), &h_info);
        if (ret != -1){
            printf("----------------------------------------");
            hash_info *head = h_info;
            while (h_info != NULL){
                retrieve_elements(h_info->path, &h_info->passed_hash, &h_info->hash, &h_info->game);
                h_info = h_info->next;
            }
            h_info = head; // Reset h_info to point to the head of the list
            while (h_info != NULL){
                printf("Hash: %s\n", h_info->hash);
                printf("Game: %s\n", h_info->game);
                printf("Path: %s\n", h_info->path);
                h_info = h_info->next;
            }
            h_info = head; // Reset h_info to point to the head of the list

            if (check_internet_connection() == 1){
                char message[512];
                while (h_info != NULL){
                    sprintf(message, "Hash: %s\nGame: %s\nPath: %s\n", h_info->hash, h_info->game, h_info->path);
                    found_hash_info *info = (found_hash_info*)malloc(sizeof(found_hash_info));
                    info->found_hash = h_info->hash;
                    info->game = h_info->game;
                    info->user_id = get_user_info()->name;
                    send_passes_hash_database(info);
                    notify_me(message);
                    free(info);
                    h_info = h_info->next;
                }
                h_info = head; // Reset h_info to point to the head of the list
            }
            update_noice(get_noice_path(), cores_to_use);
        }

        for (int i = 0; i < cores_to_use; i++){
            long unsigned int arg = noice + i;
            sprintf(noice_str, "%lu", arg);
            char *args[] = {"/c", noice_str, NULL};
            spawn_process(program, args, &pid_s[i]);
            printf("Noice: %d has started\n", arg);
        }

        for (int i = 0; i < cores_to_use; i++){
            wait_for_process(pid_s[i], &statuses[i]);
            if (statuses[i] == 0) noice++;
        }
        update_noice(get_noice_path(), cores_to_use);
        clear_terminal();
    }



    return 0;
}
#pragma clang diagnostic pop