#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "helper.h"


#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif


static const char hex_lookup[] = "0123456789abcdef";

void hex_to_byte_array(const char* hex_string, uint8_t* byte_array, int hex_string_length) {
    for (int _idx = 0; _idx * 2 < hex_string_length; _idx++) {
        char c1 = hex_string[_idx * 2];
        char c2 = hex_string[_idx * 2 + 1];

        uint8_t b1 = (c1 <= '9') ? (c1 - '0') : (c1 - 'a' + 10);
        uint8_t b2 = (c2 <= '9') ? (c2 - '0') : (c2 - 'a' + 10);

        byte_array[_idx] = (b1 << 4) | b2;
    }
}

void byte_array_to_hex(const uint8_t* byte_array, char* hex_string, int byte_array_length) {
    for (int idx_ = 0; idx_ < byte_array_length; idx_++) {
        uint8_t b = byte_array[idx_];
        hex_string[idx_ * 2] = hex_lookup[b >> 4];
        hex_string[idx_ * 2 + 1] = hex_lookup[b & 0x0f];
    }
    hex_string[byte_array_length * 2] = '\0';
}

void slice(const char* str, char* buffer, int start, int end) {
    for (int i = start; i < end; i++) {
        buffer[i - start] = str[i];
    }
    buffer[end - start] = '\0';
}

void reverse(char *str, int length)
{
    char *start = str;
    char *end = str + length - 1;
    while (start < end)
    {
        char ch = *start;
        *start++ = *end;
        *end-- = ch;
    }
}

char* get_home_dir() {
    char* home_dir;

#ifdef _WIN32
    home_dir = getenv("APPDATA");
#else
    home_dir = getenv("HOME");
#endif

    if(home_dir == NULL) {
        fprintf(stderr, "Error: Could not find home directory.\n");
        return NULL;
    }

    return home_dir;
}

char* get_app_dir() {
    char* home_dir = get_home_dir();
    if (home_dir == NULL) {
        return NULL;
    }

#ifdef _WIN32
    char* app_dir = malloc(strlen(home_dir) + strlen("\\.HO2") + 1);
    if (app_dir == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for app_dir.\n");
        return NULL;
    }

    strcpy(app_dir, home_dir);
    strcat(app_dir, "\\.HO2");
#else
    char* app_dir = malloc(strlen(home_dir) + strlen("/.HO2") + 1);
    if (app_dir == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for app_dir.\n");
        return NULL;
    }

    strcpy(app_dir, home_dir);
    strcat(app_dir, "/.HO2");
#endif

    return app_dir;
}

char *get_user_info_path() {
    char *app_dir = get_app_dir();
    if (app_dir == NULL) {
        free(app_dir);
        return NULL;
    }
    char *user_info_path = malloc(strlen(app_dir) + strlen("\\user_info.json") + 1);
    if (user_info_path == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for user_info_path.\n");
        free(app_dir);
        free(user_info_path);
        return NULL;
    }
    sprintf(user_info_path, "%s\\user_info.json", app_dir);
    free(app_dir);
    return user_info_path;
}


char *get_hash_path() {
    char *app_dir = get_app_dir();
    if (app_dir == NULL) {
        return NULL;
    }

#ifdef _WIN32
    char *hash_path = malloc(strlen(app_dir) + strlen("\\.hash\\hash.csv") + 1);
    if (hash_path == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for hash_path.\n");
        return NULL;
    }

    strcpy(hash_path, app_dir);
    strcat(hash_path, "\\.hash\\hash.csv");
#else
    char *hash_path = malloc(strlen(app_dir) + strlen("/.hash/hash.csv") + 1);
    if (hash_path == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for hash_path.\n");
        return NULL;
    }

    strcpy(hash_path, app_dir);
    strcat(hash_path, "/.hash/hash.csv");
#endif

    return hash_path;
}

char *get_noice_path() {
    char *app_dir = get_app_dir();
    if (app_dir == NULL) {
        free(app_dir);
        return NULL;
    }
    char *noice_path = malloc(strlen(app_dir) + strlen("\\noice.txt") + 1);
    if (noice_path == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for noice_path.\n");
        free(app_dir);
        free(noice_path);
        return NULL;
    }
    sprintf(noice_path, "%s\\noice.txt", app_dir);
    free(app_dir);
    return noice_path;
}

char *get_passed_hash_dir_path() {
    char *app_dir = get_app_dir();
    create_passed_hash_dir();
    if (app_dir == NULL) {
        return NULL;
    }

#ifdef _WIN32
    char *passed_hash_dir_path = malloc(strlen(app_dir) + strlen("\\.passed_hash") + 1);
    if (passed_hash_dir_path == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for passed_hash_dir_path.\n");
        return NULL;
    }

    strcpy(passed_hash_dir_path, app_dir);
    strcat(passed_hash_dir_path, "\\.passed_hash");
#else
    char *passed_hash_dir_path = malloc(strlen(app_dir) + strlen("/.passed_hash") + 1);
    if (passed_hash_dir_path == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for passed_hash_dir_path.\n");
        return NULL;
    }

    strcpy(passed_hash_dir_path, app_dir);
    strcat(passed_hash_dir_path, "/.passed_hash");
#endif

    return passed_hash_dir_path;
}


#ifdef _WIN32

cpu_info get_cpu_info() {
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);

    DWORD BufSize = sizeof(DWORD);
    DWORD dwMHz = 0;
    HKEY hKey;
    // open the key where the proc speed is hidden:
    long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                                "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
                                0,
                                KEY_READ,
                                &hKey);
    if(lError == ERROR_SUCCESS){
        // query the key:
        RegQueryValueEx(hKey, "~MHz", NULL, NULL, (LPBYTE) &dwMHz, &BufSize);
    }

    // Get the CPU name
    char ProcessorNameString[128];
    DWORD size = sizeof(ProcessorNameString);
    if (RegQueryValueEx(hKey, "ProcessorNameString", NULL, NULL, (LPBYTE)&ProcessorNameString, &size) == ERROR_SUCCESS) {
        printf("Processor Name: %s\n", ProcessorNameString);
    }

    cpu_info info;
    info.model = ProcessorNameString; // Set the model to the CPU name
    info.cores = systemInfo.dwNumberOfProcessors;
    info.speed = (double)dwMHz;

    RegCloseKey(hKey); // Don't forget to close the key when you're done with it

    return info;
}

#else

cpu_info get_cpu_info() {
    FILE* fp;
    char buffer[128];
    cpu_info info;
    info.model = malloc(128);

    fp = popen("/usr/bin/lscpu | grep 'Model name:'", "r");
    fgets(buffer, sizeof(buffer), fp);
    slice(buffer, info.model, 12, strlen(buffer) - 1);
    pclose(fp);

    fp = popen("/usr/bin/lscpu | grep 'CPU(s):'", "r");
    fgets(buffer, sizeof(buffer), fp);
    slice(buffer, buffer, 7, strlen(buffer) - 1);
    info.cores = atoi(buffer);
    pclose(fp);

    fp = popen("/usr/bin/lscpu | grep 'CPU max MHz:'", "r");
    fgets(buffer, sizeof(buffer), fp);
    slice(buffer, buffer, 13, strlen(buffer) - 1);
    info.speed = atof(buffer);
    pclose(fp);

    return info;
}

#endif

user_info* get_user_info() {
    user_info* info = malloc(sizeof(user_info));
    char input;

    info->name = malloc(256);
    info->email = malloc(256);
    info->secret = malloc(256);
    info->phone = malloc(256);
    info->pc_name = malloc(256);

    do {


        printf("Please you must** ENTER an accurate information\n");
        printf("You must NOT to forget your secret\n");
        printf("You will need the secret and the information you provide \nto prove yourself in case you find the solution\n");


        printf("Enter your name: ");
        fgets(info->name, 256, stdin);
        info->name[strcspn(info->name, "\n")] = 0; // remove the newline character

        printf("Enter your email: ");
        fgets(info->email, 256, stdin);
        info->email[strcspn(info->email, "\n")] = 0; // remove the newline character

        printf("Enter your secret: ");
        fgets(info->secret, 256, stdin);
        info->secret[strcspn(info->secret, "\n")] = 0; // remove the newline character

        printf("Enter your phone number: ");
        fgets(info->phone, 256, stdin);
        info->phone[strcspn(info->phone, "\n")] = 0; // remove the newline character

        printf("Confirm your information\n");
        printf("Is this information correct? (y/n): ");
        scanf(" %c", &input);
        clear_terminal();

    } while (input != 'y' && input != 'Y');
#ifdef _WIN32
    DWORD buffer_length = 256;
    if (!GetComputerName(info->pc_name, &buffer_length)) {
        fprintf(stderr, "Failed to get computer name. Error: %lu\n", GetLastError());
    }
    return info;
#else

        char buffer[256];
        if (gethostname(buffer, sizeof(buffer)) == -1) {
            perror("Failed to get computer name");
        } else {
            strncpy(info->pc_name, buffer, sizeof(buffer));
        }
        return info;
#endif
}