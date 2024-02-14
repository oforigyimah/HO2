#include "helper_func/helper.h"
#include <stdio.h>

#define PRINT_LINE(TITLE) printf("\n\n*************************** %s ******************************\n\n", TITLE)


#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"
int main(){

    printf("Home dir is %s\n", get_app_dir());

    init();
    PRINT_LINE("Welcome");
    char *welcome = "Welcome to the main function";
    char *notice = "This program needs hashset to work properly.\nPlease make sure that you update the hashset at least\nonce a day";
    char c;
    int cores_to_use;
    hash_info *h_info = NULL;
    int ret;

    printf("%s\n", welcome);
    printf("%s\n", notice);

    user_info *user = get_user_info();

    printf("User Info\n");
    printf("Name: %s\n", user->name);
    printf("Secret: %s\n", user->secret);
    printf("Email: %s\n", user->email);
    printf("Phone: %s\n", user->phone);
    printf("PC Name: %s\n", user->pc_name);



    PRINT_LINE("CPU Info");
    cpu_info cpu = get_cpu_info();
    printf("Cpu model is %s\n", cpu.model);
    printf("cpu cores is %d\n", cpu.cores);
    printf("cpu clock speed is %f Mhz\n", cpu.speed);

    PRINT_LINE("Options\n");

    printf("Enter 1 to Update hashset\n");
    printf("Enter 2 to start computation\n");
    printf("Your choice: ");

    scanf("%c", &c);

    switch (c) {
        case '1':
            download_hashset(get_hash_path());
            printf("Updating hashset S \n");
        case '2':
            printf("Starting computation\n");
            break;
        default:
            printf("Invalid input\n");
            printf("starting computation\n");
    }

    printf("It's recommended not to enter a number greater than the number of cores on your cpu\n");

   int result;
do {
    printf("Enter the number of cpu cores to use ==>  ");
    result = scanf("%d%*c", &cores_to_use);
    printf("Press Enter to continue: ");
    while(getchar() != '\n'); // This line consumes the newline character
    if(result != 1) {
        printf("Invalid input, must be integer\n");
    }
} while(result != 1);

    printf("\n");
    printf("You are using %d cores of your total %d cores\n", cores_to_use, cpu.cores);


    pid_t pid_s[cores_to_use];
    int statuses[cores_to_use];
    char *program = "ho2_child.exe";

    long unsigned int  noice = 0;
    char noice_str[50];
    while (1){

        ret = store_file_paths(get_passed_hash_dir_path(), &h_info);
        if (ret != -1){
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
                    notify_me(message);
                    h_info = h_info->next;
                }
                h_info = head; // Reset h_info to point to the head of the list
            }
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
        if (noice == 100000) break;
    }



    return 0;
}
#pragma clang diagnostic pop