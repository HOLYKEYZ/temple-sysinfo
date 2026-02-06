/*
 * System Info Tool - Enhanced Edition
 * A comprehensive Windows system information utility
 * by HolyKeyz
 * 
 * Compile: tcc -o sysinfo.exe sysinfo.c -lkernel32 -ladvapi32
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <tlhelp32.h>

/* ============================================
 * Helper: Print separator
 * ============================================ */
void print_header(const char *title) {
    printf("\n  +=========================================+\n");
    printf("  |  %-37s  |\n", title);
    printf("  +=========================================+\n");
}

void print_footer(void) {
    printf("  +=========================================+\n");
}

/* ============================================
 * OS Info
 * ============================================ */
void print_os_info(void) {
    print_header("SYSTEM INFORMATION");
    
    char computer_name[256] = "Unknown";
    DWORD size = sizeof(computer_name);
    GetComputerNameA(computer_name, &size);
    printf("  |  Computer:     %-24s|\n", computer_name);
    
    char user_name[256] = "Unknown";
    size = sizeof(user_name);
    GetUserNameA(user_name, &size);
    printf("  |  User:         %-24s|\n", user_name);
    
    char win_dir[256] = "";
    GetWindowsDirectoryA(win_dir, sizeof(win_dir));
    printf("  |  Windows Dir:  %-24s|\n", win_dir);
    
    char sys_dir[256] = "";
    GetSystemDirectoryA(sys_dir, sizeof(sys_dir));
    printf("  |  System Dir:   %-24s|\n", sys_dir);
    
    print_footer();
}

/* ============================================
 * CPU Info
 * ============================================ */
void print_cpu_info(void) {
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    
    print_header("CPU INFORMATION");
    
    char *arch = "Unknown";
    if (si.wProcessorArchitecture == 9) arch = "x64 (AMD64)";
    else if (si.wProcessorArchitecture == 0) arch = "x86 (Intel)";
    else if (si.wProcessorArchitecture == 5) arch = "ARM";
    
    printf("  |  Architecture: %-24s|\n", arch);
    printf("  |  Processors:   %-24lu|\n", si.dwNumberOfProcessors);
    printf("  |  Page Size:    %-24lu|\n", si.dwPageSize);
    
    print_footer();
}

/* ============================================
 * Memory Info
 * ============================================ */
void print_memory_info(void) {
    MEMORYSTATUS ms;
    ms.dwLength = sizeof(ms);
    GlobalMemoryStatus(&ms);
    
    double total_mb = (double)ms.dwTotalPhys / (1024.0 * 1024.0);
    double avail_mb = (double)ms.dwAvailPhys / (1024.0 * 1024.0);
    double used_mb = total_mb - avail_mb;
    int percent_used = (int)ms.dwMemoryLoad;
    
    print_header("MEMORY INFORMATION");
    printf("  |  Total RAM:    %-7.0f MB              |\n", total_mb);
    printf("  |  Available:    %-7.0f MB              |\n", avail_mb);
    printf("  |  Used:         %-7.0f MB (%d%%)        |\n", used_mb, percent_used);
    printf("  |  [");
    
    int bar_len = 34;
    int filled = (percent_used * bar_len) / 100;
    int i;
    for (i = 0; i < bar_len; i++) {
        if (i < filled) printf("#");
        else printf("-");
    }
    printf("]|\n");
    print_footer();
}

/* ============================================
 * Disk Info
 * ============================================ */
void print_disk_info(void) {
    print_header("DISK INFORMATION");
    
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
                int percent = (total_gb > 0) ? (int)(((total_gb - free_gb) / total_gb) * 100) : 0;
                
                printf("  |  Drive %c: %6.1fGB total, %6.1fGB free |\n", 
                       drive_letter, total_gb, free_gb);
                printf("  |  [");
                int bar_len = 32;
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
    print_footer();
}

/* ============================================
 * System Uptime
 * ============================================ */
void print_uptime(void) {
    DWORD uptime_ms = GetTickCount();
    DWORD seconds = uptime_ms / 1000;
    DWORD minutes = seconds / 60;
    DWORD hours = minutes / 60;
    DWORD days = hours / 24;
    
    seconds %= 60;
    minutes %= 60;
    hours %= 24;
    
    print_header("SYSTEM UPTIME");
    printf("  |  Uptime: %lu days, %lu hrs, %lu min, %lu sec   |\n", 
           days, hours, minutes, seconds);
    print_footer();
}

/* ============================================
 * Environment Variables (selected)
 * ============================================ */
void print_env_vars(void) {
    print_header("ENVIRONMENT VARIABLES");
    
    char *vars[] = {"PATH", "TEMP", "USERPROFILE", "HOMEDRIVE", "PROCESSOR_ARCHITECTURE"};
    int i;
    
    for (i = 0; i < 5; i++) {
        char *val = getenv(vars[i]);
        if (val) {
            /* Truncate long values */
            char truncated[30];
            if (strlen(val) > 28) {
                strncpy(truncated, val, 25);
                truncated[25] = '\0';
                strcat(truncated, "...");
            } else {
                strcpy(truncated, val);
            }
            printf("  |  %-10s: %-26s|\n", vars[i], truncated);
        }
    }
    print_footer();
}

/* ============================================
 * Process List (top 10 by name)
 * ============================================ */
void print_process_list(void) {
    print_header("RUNNING PROCESSES (First 10)");
    
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        printf("  |  Unable to get process list           |\n");
        print_footer();
        return;
    }
    
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    
    int count = 0;
    if (Process32First(snapshot, &pe)) {
        do {
            if (count < 10) {
                char name[30];
                strncpy(name, pe.szExeFile, 28);
                name[28] = '\0';
                printf("  |  [%5lu] %-30s|\n", pe.th32ProcessID, name);
                count++;
            }
        } while (Process32Next(snapshot, &pe) && count < 10);
    }
    
    CloseHandle(snapshot);
    
    /* Count total */
    int total = 0;
    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32First(snapshot, &pe)) {
        do { total++; } while (Process32Next(snapshot, &pe));
    }
    CloseHandle(snapshot);
    
    printf("  |  ... and %d more processes              |\n", total - 10);
    print_footer();
}

/* ============================================
 * Power/Battery Status
 * ============================================ */
void print_power_status(void) {
    print_header("POWER STATUS");
    
    SYSTEM_POWER_STATUS ps;
    if (GetSystemPowerStatus(&ps)) {
        char *ac_status = "Unknown";
        if (ps.ACLineStatus == 0) ac_status = "Offline (Battery)";
        else if (ps.ACLineStatus == 1) ac_status = "Online (Plugged In)";
        
        printf("  |  AC Power:     %-24s|\n", ac_status);
        
        if (ps.BatteryFlag != 128 && ps.BatteryFlag != 255) {
            /* Has battery */
            char *charging = "";
            if (ps.BatteryFlag & 8) charging = " [Charging]";
            
            if (ps.BatteryLifePercent <= 100) {
                printf("  |  Battery:      %d%%%s                  |\n", 
                       ps.BatteryLifePercent, charging);
                
                printf("  |  [");
                int bar_len = 34;
                int filled = (ps.BatteryLifePercent * bar_len) / 100;
                int i;
                for (i = 0; i < bar_len; i++) {
                    if (i < filled) printf("#");
                    else printf("-");
                }
                printf("]|\n");
            }
        } else {
            printf("  |  Battery:      No battery detected    |\n");
        }
    } else {
        printf("  |  Unable to get power status           |\n");
    }
    print_footer();
}

/* ============================================
 * Display/Graphics Info
 * ============================================ */
void print_display_info(void) {
    print_header("DISPLAY INFORMATION");
    
    HDC hdc = GetDC(NULL);
    if (hdc) {
        int width = GetDeviceCaps(hdc, HORZRES);
        int height = GetDeviceCaps(hdc, VERTRES);
        int bits = GetDeviceCaps(hdc, BITSPIXEL);
        int planes = GetDeviceCaps(hdc, PLANES);
        int colors = bits * planes;
        
        printf("  |  Resolution:   %d x %d               |\n", width, height);
        printf("  |  Color Depth:  %d-bit                   |\n", colors);
        printf("  |  Refresh Rate: %d Hz                    |\n", 
               GetDeviceCaps(hdc, VREFRESH));
        
        ReleaseDC(NULL, hdc);
    } else {
        printf("  |  Unable to get display info           |\n");
    }
    print_footer();
}

/* ============================================
 * Main Entry Point
 * ============================================ */
int main(void) {
    printf("\n");
    printf("  +=========================================+\n");
    printf("  |                                         |\n");
    printf("  |   SYSTEM INFORMATION TOOL - ENHANCED   |\n");
    printf("  |            by HolyKeyz                 |\n");
    printf("  |                                         |\n");
    printf("  +=========================================+\n");
    
    print_os_info();
    print_cpu_info();
    print_memory_info();
    print_disk_info();
    print_uptime();
    print_power_status();
    print_display_info();
    print_process_list();
    print_env_vars();
    
    printf("\n  Press Enter to exit...");
    getchar();
    
    return 0;
}
