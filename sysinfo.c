/*
 * System Info Tool
 * A Windows system information utility
 * Compile: tcc -o sysinfo.exe sysinfo.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/* ============================================
 * CPU Info
 * ============================================ */
void print_cpu_info(void) {
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    
    printf("\n");
    printf("  +=========================================+\n");
    printf("  |           CPU INFORMATION              |\n");
    printf("  +=========================================+\n");
    
    char *arch = "Unknown";
    if (si.wProcessorArchitecture == 9) arch = "x64 (AMD64)";
    else if (si.wProcessorArchitecture == 0) arch = "x86 (Intel)";
    else if (si.wProcessorArchitecture == 5) arch = "ARM";
    
    printf("  |  Architecture: %-24s|\n", arch);
    printf("  |  Processors:   %-24lu|\n", si.dwNumberOfProcessors);
    printf("  |  Page Size:    %-24lu|\n", si.dwPageSize);
    printf("  +=========================================+\n");
}

/* ============================================
 * Memory Info (using older API for TCC compat)
 * ============================================ */
void print_memory_info(void) {
    MEMORYSTATUS ms;
    ms.dwLength = sizeof(ms);
    GlobalMemoryStatus(&ms);
    
    double total_mb = (double)ms.dwTotalPhys / (1024.0 * 1024.0);
    double avail_mb = (double)ms.dwAvailPhys / (1024.0 * 1024.0);
    double used_mb = total_mb - avail_mb;
    int percent_used = (int)ms.dwMemoryLoad;
    
    printf("\n");
    printf("  +=========================================+\n");
    printf("  |          MEMORY INFORMATION            |\n");
    printf("  +=========================================+\n");
    printf("  |  Total RAM:    %-7.0f MB              |\n", total_mb);
    printf("  |  Available:    %-7.0f MB              |\n", avail_mb);
    printf("  |  Used:         %-7.0f MB (%d%%)        |\n", used_mb, percent_used);
    printf("  |                                        |\n");
    printf("  |  [");
    
    int bar_len = 30;
    int filled = (percent_used * bar_len) / 100;
    int i;
    for (i = 0; i < bar_len; i++) {
        if (i < filled) printf("#");
        else printf("-");
    }
    printf("]  |\n");
    printf("  +=========================================+\n");
}

/* ============================================
 * Disk Info
 * ============================================ */
void print_disk_info(void) {
    printf("\n");
    printf("  +=========================================+\n");
    printf("  |           DISK INFORMATION             |\n");
    printf("  +=========================================+\n");
    
    /* Check common drive letters */
    char drive_letter;
    char drive_path[4];
    DWORD sectors_per_cluster, bytes_per_sector, free_clusters, total_clusters;
    
    for (drive_letter = 'C'; drive_letter <= 'Z'; drive_letter++) {
        sprintf(drive_path, "%c:\\", drive_letter);
        
        if (GetDriveTypeA(drive_path) == DRIVE_FIXED) {
            if (GetDiskFreeSpaceA(drive_path, &sectors_per_cluster, &bytes_per_sector, 
                                  &free_clusters, &total_clusters)) {
                double bytes_per_cluster = (double)sectors_per_cluster * bytes_per_sector;
                double total_gb = (bytes_per_cluster * total_clusters) / (1024.0 * 1024.0 * 1024.0);
                double free_gb = (bytes_per_cluster * free_clusters) / (1024.0 * 1024.0 * 1024.0);
                double used_gb = total_gb - free_gb;
                int percent = (total_gb > 0) ? (int)((used_gb / total_gb) * 100) : 0;
                
                printf("  |  Drive %c:                               |\n", drive_letter);
                printf("  |    Total: %6.1f GB | Free: %6.1f GB   |\n", total_gb, free_gb);
                printf("  |    [");
                int bar_len = 28;
                int filled = (percent * bar_len) / 100;
                int i;
                for (i = 0; i < bar_len; i++) {
                    if (i < filled) printf("#");
                    else printf("-");
                }
                printf("] %3d%%|\n", percent);
            }
        }
    }
    printf("  +=========================================+\n");
}

/* ============================================
 * OS Info
 * ============================================ */
void print_os_info(void) {
    printf("\n");
    printf("  +=========================================+\n");
    printf("  |         SYSTEM INFORMATION             |\n");
    printf("  +=========================================+\n");
    
    /* Computer name */
    char computer_name[256] = "Unknown";
    DWORD size = sizeof(computer_name);
    GetComputerNameA(computer_name, &size);
    printf("  |  Computer:     %-24s|\n", computer_name);
    
    /* User name */
    char user_name[256] = "Unknown";
    size = sizeof(user_name);
    GetUserNameA(user_name, &size);
    printf("  |  User:         %-24s|\n", user_name);
    
    /* Windows directory */
    char win_dir[256] = "";
    GetWindowsDirectoryA(win_dir, sizeof(win_dir));
    printf("  |  Windows Dir:  %-24s|\n", win_dir);
    
    /* System directory */
    char sys_dir[256] = "";
    GetSystemDirectoryA(sys_dir, sizeof(sys_dir));
    printf("  |  System Dir:   %-24s|\n", sys_dir);
    
    printf("  +=========================================+\n");
}

/* ============================================
 * Main Entry Point
 * ============================================ */
int main(void) {
    printf("\n");
    printf("  +=========================================+\n");
    printf("  |                                         |\n");
    printf("  |      SYSTEM INFORMATION TOOL           |\n");
    printf("  |           by HolyKeyz                  |\n");
    printf("  |                                         |\n");
    printf("  +=========================================+\n");
    
    print_os_info();
    print_cpu_info();
    print_memory_info();
    print_disk_info();
    
    printf("\n  Press Enter to exit...");
    getchar();
    
    return 0;
}
