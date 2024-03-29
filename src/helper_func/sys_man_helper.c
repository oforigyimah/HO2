#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
    #include <intrin.h>
    #include <process.h>
#else
    #include <unistd.h>
    #include <sys/sysinfo.h>
    #include <sys/wait.h>
#endif

int not_main() {
    #ifdef _WIN32
        // Windows-specific code
        char computerName[MAX_COMPUTERNAME_LENGTH + 1];
        DWORD size = sizeof(computerName) / sizeof(computerName[0]);
        GetComputerName(computerName, &size);
        printf("Computer Name: %s\n", computerName);

        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        printf("Number of Cores: %u\n", sysinfo.dwNumberOfProcessors);

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
        // convert the DWORD to a CString:
        printf("CPU Speed: %u MHz\n", dwMHz);
    #else
        // Linux-specific code
        char hostname[1024];
        gethostname(hostname, 1024);
        printf("Hostname: %s\n", hostname);

        FILE* fp;
        char buffer[128];
        fp = popen("/usr/bin/lscpu | grep 'Model name:'", "r");
        fgets(buffer, sizeof(buffer), fp);
        printf("CPU Model: %s", buffer);
        pclose(fp);

        fp = popen("/usr/bin/lscpu | grep 'CPU(s):'", "r");
        fgets(buffer, sizeof(buffer), fp);
        printf("CPU Cores: %s", buffer);
        pclose(fp);


    #endif

    return 0;
}

void spawn_process(char* program, char** args, pid_t* pid) {
#ifdef _WIN32
    // Windows-specific code
    *pid = _spawnv(_P_NOWAIT, program, (const char *const *) args);
    if (*pid == -1) {
        perror("Error spawning process");
    }
#else
    // Linux-specific code
    *pid = fork();
    if (*pid == -1) {
        perror("Error forking process");
    } else if (*pid == 0) {
        if (execvp(program, args) == -1) {
            perror("Error executing program");
            printf("Press Enter to continue: ");
            while(getchar() != '\n');
        }
    }
#endif
}
void wait_for_process(pid_t pid, int* status){
#ifdef _WIN32
    // Windows-specific code
    if (_cwait(status, pid, 0) == -1) {
        perror("Child process did not exit normally");
    }
#else
    // Linux-specific code
    waitpid(pid, status, 0);
#endif
}

#ifdef _WIN32
void set_console_mode() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;

    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#endif

void clear_terminal() {
#ifdef _WIN32
    system("cls");
    set_console_mode();
#else
    system("clear");
#endif
   printf("\033[1;32mFor any enquires, please contact \"0559803733\" on WhatsApp\\Telegram\n\033[0m");
}