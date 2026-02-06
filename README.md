<p align="center">
  <img src="holyc.png" alt="HolyC Logo" width="200"/>
</p>

# System Info Tool

A Windows system information utility written in **C** with a **HolyC** syntax demonstration.

## Why?

Checking system info across different Windows menus is stressful — Task Manager for processes, Settings for specs, Control Panel for this and that. This tool shows everything in one place.

The HolyC version exists out of reverence for **Terry A. Davis**, the genius behind TempleOS. 🙏

## Features

| Module | Description |
|--------|-------------|
| 🖥️ **System** | Computer name, user, directories |
| ⚡ **CPU** | Architecture, cores, page size |
| 🧠 **Memory** | RAM usage with visual bar |
| 💾 **Disk** | Per-drive capacity and usage |
| ⏱️ **Uptime** | Time since last boot |
| 🔋 **Power** | AC/battery status |
| 🖼️ **Display** | Resolution, color depth |
| 📋 **Processes** | Running processes list |
| 🌍 **Environment** | Key environment variables |

## Build & Run

Requires [TCC (Tiny C Compiler)](https://bellard.org/tcc/):

```bash
tcc -o sysinfo.exe sysinfo.c -lkernel32 -ladvapi32 -luser32 -lgdi32
.\sysinfo.exe
```

## Project Structure

```
├── sysinfo.c            # C source (Windows)
├── sysinfo.exe          # Compiled executable
├── holyc/
│   ├── sysinfo.HC       # HolyC syntax demo
│   └── temple_sysinfo.HC # Real TempleOS version
├── holyc.png            # HolyC logo
└── README.md
```

## About HolyC

The `holyc/` folder contains a **HolyC** version of this tool. HolyC is the programming language created by Terry A. Davis for [TempleOS](https://templeos.org/), featuring:

- Direct hardware access (Ring 0)
- JIT compilation
- Simplified C-like syntax
- Built-in graphics and sound

The HolyC version serves as a syntax reference and tribute to Terry's work.

## License

MIT

---

*Built with ❤️ by HolyKeyz*
