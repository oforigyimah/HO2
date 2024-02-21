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

typedef struct {
    char *model;
    int cores;
    double speed;
} cpu_info;

typedef struct hash_info {
    char *passed_hash;
    char *hash;
    char *game;
    char *path;
    struct hash_info *next;
    int index;
} hash_info;

typedef struct {
    char *name;
    char *secret;
    char *email;
    char *phone;
    char *pc_name;
    char *user_id;
} user_info;

typedef struct {
    unsigned int start;
    unsigned int end;
    int status;
} noice_info;

typedef struct {
    char *user_id;
    char *found_hash;
    char *game;
} found_hash_info;

struct string {
    char *ptr;
    size_t len;
};


// Function prototypes of files_helper.c
long unsigned int get_noice(char *filepath);
void update_noice(char *filepath, int num);
int get_hash(char *filepath, char *hashes[MAX_HASHES], char *games[MAX_HASHES]);
int handle_passed_hash(char *passed_hash,char *hash_str, char *path, int index);
int file_exists(const char *filename);
int create_app_dir();
int create_hash_dir();
int create_passed_hash_dir();
int create_noice_dir();
int store_file_paths(const char *dir_path, hash_info **head);
void retrieve_elements(const char* path, char** passed_hash, char** hash, char** game);
void init();
void write_user_info_to_json(user_info *info, const char *filename);
user_info* read_user_info_from_json(const char *filename);

// Function prototypes of string_helper.c
void hex_to_byte_array(const char* hex_sting, uint8_t* byte_array, int hex_string_length);
void byte_array_to_hex(const uint8_t* byte_array, char* hex_string, int byte_array_length);
void slice(const char* input, char* output, int start, int end);
void reverse(char* input, int length);
char* get_home_dir();
char* get_app_dir();
char *get_hash_path();
char *get_passed_hash_dir_path();
char *get_user_info_path();
char *get_noice_dir();
char *get_noice_path();
cpu_info get_cpu_info();
user_info* get_user_info();
void init_string(struct string *s);
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s);
void replace_plus_with_space(char *str);
// Function prototypes of sys_mam_helper.c
void spawn_process(char* program, char** args, pid_t* pid);
void wait_for_process(pid_t pid, int* status);
void clear_terminal();
void prompt_hashset_missing();
void prompt_noice_missing();


// Function prototypes of net_helper.c
int check_internet_connection();
void download_hashset(char *path);
int notify_me(char* message);
int request_noice(char *filename);
int send_passes_hash_database(found_hash_info *info);

#endif // HELPER_H