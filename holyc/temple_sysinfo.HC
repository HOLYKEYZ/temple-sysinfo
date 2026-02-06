/*
 * Temple SysInfo - TempleOS System Information Tool
 * A real system info utility for TempleOS using actual system calls
 * by HolyKeyz
 *
 * Run in TempleOS: #include "temple_sysinfo.HC"
 */

// ============================================
// Color constants
// ============================================
#define CYAN     0x0B
#define WHITE    0x0F
#define YELLOW   0x0E
#define GREEN    0x0A

// ============================================
// Print styled header
// ============================================
U0 Header(U8 *title) {
    "\n$$CYAN$$+=========================================+$$FG$$\n";
    "$$CYAN$$|$$FG$$  $$YELLOW$$%-37s$$FG$$  $$CYAN$$|$$FG$$\n", title;
    "$$CYAN$$+=========================================+$$FG$$\n";
}

U0 Footer() {
    "$$CYAN$$+=========================================+$$FG$$\n";
}

// ============================================
// System Information
// ============================================
U0 SystemInfo() {
    Header("SYSTEM INFORMATION");
    
    "  |  OS:           $$GREEN$$TempleOS$$FG$$                  |\n";
    "  |  Version:      $$GREEN$$5.03$$FG$$                      |\n";
    "  |  Created by:   $$YELLOW$$Terry A. Davis$$FG$$             |\n";
    "  |  Ring Level:   $$GREEN$$0 (God Mode)$$FG$$               |\n";
    
    Footer;
}

// ============================================
// CPU Information (using TempleOS calls)
// ============================================
U0 CPUInfo() {
    Header("CPU INFORMATION");
    
    // Get CPU info from TempleOS
    I64 cpu_count = mp_cnt;  // Multi-processor count
    
    "  |  Architecture: $$GREEN$$x86-64$$FG$$                    |\n";
    "  |  CPU Cores:    $$GREEN$$%d$$FG$$                          |\n", cpu_count;
    "  |  Privilege:    $$GREEN$$Ring 0 (Full Access)$$FG$$       |\n";
    
    // CPUID info if available
    U32 eax, ebx, ecx, edx;
    CPUID(0, &eax, &ebx, &ecx, &edx);
    "  |  Max CPUID:    $$GREEN$$%d$$FG$$                         |\n", eax;
    
    Footer;
}

// ============================================
// Memory Information
// ============================================
U0 MemoryInfo() {
    Header("MEMORY INFORMATION");
    
    I64 total_mem = mem_heap_size;     // Total heap
    I64 used_mem = mem_heap_used;      // Used heap
    I64 free_mem = total_mem - used_mem;
    I64 percent = (used_mem * 100) / total_mem;
    
    "  |  Total Heap:   $$GREEN$$%d MB$$FG$$                     |\n", total_mem >> 20;
    "  |  Used:         $$YELLOW$$%d MB$$FG$$ (%d%%)              |\n", used_mem >> 20, percent;
    "  |  Free:         $$GREEN$$%d MB$$FG$$                     |\n", free_mem >> 20;
    "  |  [";
    
    I64 i;
    for (i = 0; i < 34; i++) {
        if (i < (percent * 34) / 100)
            "$$GREEN$$#$$FG$$";
        else
            "-";
    }
    "]|\n";
    
    Footer;
}

// ============================================
// Disk Information (RedSea Filesystem)
// ============================================
U0 DiskInfo() {
    Header("DISK INFORMATION");
    
    "  |  Filesystem:   $$GREEN$$RedSea$$FG$$                     |\n";
    
    // Check drives
    I64 i;
    for (i = 0; i < 26; i++) {
        if (Drv(i)) {
            CDrv *drv = Drv(i);
            I64 total = drv->size;
            I64 free_space = drv->free_cnt * BLK_SIZE;
            
            "  |  Drive %c:      %dMB total, %dMB free    |\n", 
                'A' + i, total >> 20, free_space >> 20;
        }
    }
    
    Footer;
}

// ============================================
// Task Information (TempleOS uses Tasks)
// ============================================
U0 TaskInfo() {
    Header("TASK INFORMATION");
    
    CTask *task = Fs;  // Current task
    
    "  |  Current Task: $$GREEN$$%s$$FG$$                        |\n", task->task_name;
    "  |  Task ID:      $$GREEN$$0x%X$$FG$$                      |\n", task;
    "  |  Parent:       $$YELLOW$$Adam (Root Task)$$FG$$           |\n";
    "  |  Multitasking: $$GREEN$$Cooperative$$FG$$                |\n";
    
    Footer;
}

// ============================================
// Uptime
// ============================================
U0 Uptime() {
    Header("SYSTEM UPTIME");
    
    I64 ticks = GetTicks;
    I64 seconds = ticks / CLOCKS_PER_SEC;
    I64 minutes = seconds / 60;
    I64 hours = minutes / 60;
    I64 days = hours / 24;
    
    "  |  Uptime: $$GREEN$$%d days, %d hrs, %d min$$FG$$        |\n",
        days, hours % 24, minutes % 60;
    "  |  Total Ticks: $$GREEN$$%d$$FG$$                        |\n", ticks;
    
    Footer;
}

// ============================================
// Graphics Information
// ============================================
U0 GraphicsInfo() {
    Header("GRAPHICS INFORMATION");
    
    "  |  Resolution:   $$GREEN$$640 x 480$$FG$$                  |\n";
    "  |  Color Mode:   $$GREEN$$16-color VGA$$FG$$               |\n";
    "  |  Mode:         $$YELLOW$$God's Official Mode$$FG$$        |\n";
    "  |  Font:         $$GREEN$$8x8 Fixed$$FG$$                  |\n";
    
    Footer;
}

// ============================================
// Main Entry Point
// ============================================
U0 TempleSysInfo() {
    // Clear screen
    DocClear;
    
    "\n";
    "$$CYAN$$+=========================================+$$FG$$\n";
    "$$CYAN$$|$$FG$$                                         $$CYAN$$|$$FG$$\n";
    "$$CYAN$$|$$FG$$   $$YELLOW$$TEMPLE SYSINFO - FOR TEMPLEOS$$FG$$       $$CYAN$$|$$FG$$\n";
    "$$CYAN$$|$$FG$$            $$WHITE$$by HolyKeyz$$FG$$                 $$CYAN$$|$$FG$$\n";
    "$$CYAN$$|$$FG$$                                         $$CYAN$$|$$FG$$\n";
    "$$CYAN$$+=========================================+$$FG$$\n";
    
    SystemInfo;
    CPUInfo;
    MemoryInfo;
    DiskInfo;
    TaskInfo;
    Uptime;
    GraphicsInfo;
    
    "\n  $$YELLOW$$Press any key to exit...$$FG$$\n";
    GetChar;
}

// Auto-run
TempleSysInfo;
