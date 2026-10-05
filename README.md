# 🐧 Linux Capstone File Explorer

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white">
  <img src="https://img.shields.io/badge/Linux-System%20Programming-111111?style=for-the-badge&logo=linux&logoColor=white">
  <img src="https://img.shields.io/badge/Kernel-Character%20Driver-555555?style=for-the-badge&logo=linux&logoColor=white">
  <img src="https://img.shields.io/badge/Platform-WSL2-4D4D4D?style=for-the-badge&logo=linux&logoColor=white">
</p>

<p align="center">
  <strong>A terminal-based Linux File Explorer integrated with a custom Linux Character Device Driver.</strong>
</p>

<p align="center">
  <sub>
    C++ • POSIX/Linux System Programming • Linux Kernel Modules • Character Devices • IOCTL
  </sub>
</p>

---

## 🧭 Overview

**Linux Capstone File Explorer** is a terminal-based file management application developed in **C++ for Linux**.

The project combines two layers:

- **User Space:** A C++ file explorer that performs Linux filesystem operations.
- **Kernel Space:** A custom Linux Character Device Driver that exposes a device interface at `/dev/capstone_monitor`.

The File Explorer communicates with the kernel driver through standard Linux device-file operations and `ioctl()` commands.

This makes the project more than a conventional file manager. It demonstrates how a Linux application can communicate with a custom kernel module through a character device interface.

---

## 🎯 Project Objectives

The project was designed around the following Linux and system-programming concepts:

- Linux filesystem operations
- POSIX system calls
- Directory traversal
- File metadata management
- File permissions
- Process and system interaction
- Linux device files
- Character device drivers
- Kernel module lifecycle
- `file_operations`
- `ioctl()` communication
- User-space / kernel-space interaction
- C++ application development
- Software architecture

---

# 🏗️ System Architecture

The project follows a layered architecture.

```text
                    ┌──────────────────────────┐
                    │       USER SPACE         │
                    │                          │
                    │   C++ File Explorer      │
                    │                          │
                    │  File / Directory Ops    │
                    │  Search / Copy / Move    │
                    │  Permissions / Metadata  │
                    └────────────┬─────────────┘
                                 │
                                 │ open()
                                 │ read()
                                 │ ioctl()
                                 ▼
                    ┌──────────────────────────┐
                    │      DEVICE FILE         │
                    │                          │
                    │ /dev/capstone_monitor    │
                    └────────────┬─────────────┘
                                 │
                                 ▼
                    ┌──────────────────────────┐
                    │       KERNEL SPACE       │
                    │                          │
                    │ Linux Character Driver   │
                    │                          │
                    │  open()                  │
                    │  read()                  │
                    │  ioctl()                │
                    │  release()              │
                    └────────────┬─────────────┘
                                 │
                                 ▼
                    ┌──────────────────────────┐
                    │      LINUX KERNEL        │
                    │                          │
                    │ Device Management        │
                    │ Character Device Layer   │
                    └──────────────────────────┘
