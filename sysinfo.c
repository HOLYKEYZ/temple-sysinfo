/*
 * System Info Tool
 * A Windows system information utility
 * Compile: tcc -o sysinfo.exe sysinfo.c
 */

#include <stdio.h>
#include <windows.h>

/* ============================================
 * CPU Info
 * ============================================ */
void print_cpu_info(void) {
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    
    printf("\n");
    printf("  ╔═══════════════════════════════════════╗\n");
    printf("  ║           CPU INFORMATION             ║\n");
    printf("  ╠═══════════════════════════════════════╣\n");
    
    char *arch;
    switch (si.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64: arch = "x64 (AMD64)"; break;
        case PROCESSOR_ARCHITECTURE_INTEL: arch = "x86 (Intel)"; break;
        case PROCESSOR_ARCHITECTURE_ARM:   arch = "ARM"; break;
        default: arch = "Unknown"; break;
    }
    
    printf("  ║  Architecture: %-22s  ║\n", arch);
    printf("  ║  Processors:   %-22lu  ║\n", si.dwNumberOfProcessors);
    printf("  ║  Page Size:    %-22lu  ║\n", si.dwPageSize);
    printf("  ╚═══════════════════════════════════════╝\n");
}

/* ============================================
 * Memory Info
 * ============================================ */
void print_memory_info(void) {
    MEMORYSTATUSEX ms;
    ms.dwLength = sizeof(ms);
    GlobalMemoryStatusEx(&ms);
    
    double total_gb = (double)ms.ullTotalPhys / (1024.0 * 1024.0 * 1024.0);
    double avail_gb = (double)ms.ullAvailPhys / (1024.0 * 1024.0 * 1024.0);
    double used_gb = total_gb - avail_gb;
    int percent_used = (int)ms.dwMemoryLoad;
    
    printf("\n");
    printf("  ╔═══════════════════════════════════════╗\n");
    printf("  ║          MEMORY INFORMATION           ║\n");
    printf("  ╠═══════════════════════════════════════╣\n");
    printf("  ║  Total RAM:    %-6.2f GB              ║\n", total_gb);
    printf("  ║  Available:    %-6.2f GB              ║\n", avail_gb);
    printf("  ║  Used:         %-6.2f GB (%d%%)        ║\n", used_gb, percent_used);
    printf("  ║                                       ║\n");
    printf("  ║  [");
    
    int bar_len = 30;
    int filled = (percent_used * bar_len) / 100;
    for (int i = 0; i < bar_len; i++) {
        if (i < filled) printf("#");
        else printf("-");
    }
    printf("] ║\n");
    printf("  ╚═══════════════════════════════════════╝\n");
}

/* ============================================
 * Disk Info
 * ============================================ */
void print_disk_info(void) {
    printf("\n");
    printf("  ╔═══════════════════════════════════════╗\n");
    printf("  ║           DISK INFORMATION            ║\n");
    printf("  ╠═══════════════════════════════════════╣\n");
    
    char drives[256];
    GetLogicalDriveStringsA(256, drives);
    
    char *drive = drives;
    while (*drive) {
        ULARGE_INTEGER free_bytes, total_bytes, total_free;
        
        if (GetDiskFreeSpaceExA(drive, &free_bytes, &total_bytes, &total_free)) {
            double total_gb = (double)total_bytes.QuadPart / (1024.0 * 1024.0 * 1024.0);
            double free_gb = (double)free_bytes.QuadPart / (1024.0 * 1024.0 * 1024.0);
            double used_gb = total_gb - free_gb;
            int percent = (int)((used_gb / total_gb) * 100);
            
            printf("  ║  Drive %c:                              ║\n", drive[0]);
            printf("  ║    Total: %7.1f GB | Free: %6.1f GB ║\n", total_gb, free_gb);
            printf("  ║    [");
            int bar_len = 28;
            int filled = (percent * bar_len) / 100;
            for (int i = 0; i < bar_len; i++) {
                if (i < filled) printf("#");
                else printf("-");
            }
            printf("] %3d%% ║\n", percent);
        }
        
        drive += strlen(drive) + 1;
    }
    printf("  ╚═══════════════════════════════════════╝\n");
}

/* ============================================
 * OS Info
 * ============================================ */
void print_os_info(void) {
    printf("\n");
    printf("  ╔═══════════════════════════════════════╗\n");
    printf("  ║         SYSTEM INFORMATION            ║\n");
    printf("  ╠═══════════════════════════════════════╣\n");
    
    // Computer name
    char computer_name[256];
    DWORD size = sizeof(computer_name);
    GetComputerNameA(computer_name, &size);
    printf("  ║  Computer:     %-22s  ║\n", computer_name);
    
    // User name
    char user_name[256];
    size = sizeof(user_name);
    GetUserNameA(user_name, &size);
    printf("  ║  User:         %-22s  ║\n", user_name);
    
    // Windows directory
    char win_dir[256];
    GetWindowsDirectoryA(win_dir, sizeof(win_dir));
    printf("  ║  Windows Dir:  %-22s  ║\n", win_dir);
    
    // System directory  
    char sys_dir[256];
    GetSystemDirectoryA(sys_dir, sizeof(sys_dir));
    printf("  ║  System Dir:   %-22s  ║\n", sys_dir);
    
    printf("  ╚═══════════════════════════════════════╝\n");
}

/* ============================================
 * Main Entry Point
 * ============================================ */
int main(void) {
    printf("\n");
    printf("  ╔═══════════════════════════════════════╗\n");
    printf("  ║                                       ║\n");
    printf("  ║      SYSTEM INFORMATION TOOL          ║\n");
    printf("  ║           by HolyKeyz                 ║\n");
    printf("  ║                                       ║\n");
    printf("  ╚═══════════════════════════════════════╝\n");
    
    print_os_info();
    print_cpu_info();
    print_memory_info();
    print_disk_info();
    
    printf("\n  Press Enter to exit...");
    getchar();
    
    return 0;
}
