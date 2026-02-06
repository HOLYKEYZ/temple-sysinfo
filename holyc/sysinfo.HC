/*
 * System Info Tool - HolyC Version
 * A demonstration of HolyC syntax for system information
 * by HolyKeyz
 *
 * NOTE: This requires the HolyC compiler to run.
 * This serves as a reference/syntax demo alongside the C version.
 */

// HolyC uses different types and conventions
// U8 = unsigned 8-bit, I64 = signed 64-bit, U64 = unsigned 64-bit

U0 PrintHeader(U8 *title) {
    "\n  +=========================================+\n";
    "  |  %-37s  |\n", title;
    "  +=========================================+\n";
}

U0 PrintFooter() {
    "  +=========================================+\n";
}

// ============================================
// System Information
// ============================================
U0 PrintSystemInfo() {
    PrintHeader("SYSTEM INFORMATION");
    
    // In HolyC, we'd use TempleOS system calls
    // This is pseudo-code for demonstration
    "  |  OS:           TempleOS / HolyC        |\n";
    "  |  Architecture: x86_64                  |\n";
    "  |  Ring:         Ring 0 (God Mode)       |\n";
    
    PrintFooter;
}

// ============================================
// CPU Information
// ============================================
U0 PrintCPUInfo() {
    PrintHeader("CPU INFORMATION");
    
    I64 num_cores = 1; // TempleOS is single-core by design
    
    "  |  Architecture: x86-64                  |\n";
    "  |  Cores:        %d                        |\n", num_cores;
    "  |  Mode:         Real Mode (Ring 0)      |\n";
    
    PrintFooter;
}

// ============================================
// Memory Information  
// ============================================
U0 PrintMemoryInfo() {
    PrintHeader("MEMORY INFORMATION");
    
    // In TempleOS, memory is directly accessible
    // MAlloc and Free are the memory functions
    
    I64 total_mem = 0x10000000; // Example: 256MB
    I64 used_mem = 0x8000000;   // Example: 128MB
    I64 percent = (used_mem * 100) / total_mem;
    
    "  |  Total RAM:    %d MB                   |\n", total_mem >> 20;
    "  |  Used:         %d MB (%d%%)             |\n", used_mem >> 20, percent;
    "  |  [";
    
    I64 i;
    for (i = 0; i < 34; i++) {
        if (i < (percent * 34) / 100)
            "#";
        else
            "-";
    }
    "]|\n";
    
    PrintFooter;
}

// ============================================
// Disk Information
// ============================================
U0 PrintDiskInfo() {
    PrintHeader("DISK INFORMATION");
    
    // TempleOS uses RedSea filesystem
    "  |  Filesystem:   RedSea                  |\n";
    "  |  Drive C:      Available               |\n";
    "  |  Drive D:      Available               |\n";
    
    PrintFooter;
}

// ============================================
// Uptime
// ============================================
U0 PrintUptime() {
    PrintHeader("SYSTEM UPTIME");
    
    // In HolyC, we'd use system ticks
    // I64 ticks = GetTicks;
    
    "  |  System has been running...            |\n";
    "  |  (Use GetTicks for actual uptime)      |\n";
    
    PrintFooter;
}

// ============================================
// Process Information
// ============================================
U0 PrintProcesses() {
    PrintHeader("TASK INFORMATION");
    
    // TempleOS uses Tasks, not processes
    "  |  TempleOS uses cooperative multitask   |\n";
    "  |  Current Task: SysInfo                 |\n";
    "  |  System Task:  Adam (root task)        |\n";
    
    PrintFooter;
}

// ============================================
// Graphics Information
// ============================================
U0 PrintGraphics() {
    PrintHeader("GRAPHICS INFORMATION");
    
    // TempleOS uses 640x480 VGA mode
    "  |  Resolution:   640 x 480               |\n";
    "  |  Color Mode:   16-color VGA            |\n";
    "  |  Graphics:     God's Official Mode     |\n";
    
    PrintFooter;
}

// ============================================
// Main Entry Point
// ============================================
U0 Main() {
    "\n";
    "  +=========================================+\n";
    "  |                                         |\n";
    "  |   SYSTEM INFO TOOL - HOLYC VERSION     |\n";
    "  |            by HolyKeyz                 |\n";
    "  |                                         |\n";
    "  +=========================================+\n";
    
    PrintSystemInfo;
    PrintCPUInfo;
    PrintMemoryInfo;
    PrintDiskInfo;
    PrintUptime;
    PrintProcesses;
    PrintGraphics;
    
    "\n  Press any key to exit...\n";
    GetChar;
}

// Auto-run Main
Main;
