#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
    #include <intrin.h>
#else
    #include <unistd.h>
    #include <sys/sysinfo.h>
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