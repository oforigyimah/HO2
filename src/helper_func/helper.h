#ifndef HELPER_H
#define HELPER_H

#ifdef _WIN32
#include <process.h>
#else
    #include <unistd.h>
#endif
#include <sys/types.h>
#include <stdint.h>


#define MAX_HASHES 30

// Function prototypes of files_helper.c
long unsigned int get_noice(char *filepath);
void update_noice(char *filepath, int num);
int get_hash(char *filepath, char *hashes[MAX_HASHES], char *games[MAX_HASHES]);
int handle_passed_hash(char *passed_hash,char *hash_str, char *path, int index);
int file_exists(const char *filename);
int create_app_dir();
int create_hash_dir();
int create_passed_hash_dir();

// Function prototypes of string_helper.c
void hex_to_byte_array(const char* hex_sting, uint8_t* byte_array, int hex_string_length);
void byte_array_to_hex(const uint8_t* byte_array, char* hex_string, int byte_array_length);
void slice(const char* input, char* output, int start, int end);
void reverse(char* input, int length);
char* get_home_dir();
char* get_app_dir();
char *get_hash_path();

// Function prototypes of sys_mam_helper.c
void spawn_process(char* program, char** args, pid_t* pid);
void wait_for_process(pid_t pid, int* status);


// Function prototypes of net_helper.c
int check_internet_connection();
void download_hashset(char *path);

#endif // HELPER_H