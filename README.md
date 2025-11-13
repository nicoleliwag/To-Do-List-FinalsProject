# To-Do List Manager: Technical Report
## C Programming with Assembly Integration

---

## Executive Summary

This To-Do List Manager is a console-based application developed in C with integrated x86 assembly language functions. The program helps users organize tasks across multiple lists, track deadlines, monitor completion status, and provides persistent storage for saving data across sessions.

---

## 1. Program Overview

### Key Features
- **Multi-List Management**: Support for up to 20 separate task lists
- **Task Organization**: Up to 50 tasks per list with descriptions and deadlines
- **Intelligent Sorting**: Automatic sorting by deadline with completed tasks moved to bottom
- **Overdue Detection**: Real-time identification of tasks past their deadline
- **Data Persistence**: Binary file storage for saving/loading application state
- **Assembly Integration**: Low-level assembly functions for arithmetic operations

---

## 2. Data Structures

**Task Structure:**
- Description (100 characters max)
- Deadline in YYYY-MM-DD format
- Completion status (complete/incomplete)
- Unix timestamp for sorting purposes

**Folder Structure:**
- List name (100 characters max)
- Array of tasks (maximum 50 tasks)
- Current task count

---

## 3. Assembly Language Integration

The program integrates three x86 assembly functions demonstrating low-level programming:

1. **asm_add(int a, int b)** - Addition operation
2. **asm_subtract(int a, int b)** - Subtraction operation  
3. **asm_increment(int a)** - Increment operation

### Cross-Platform Support
- **Microsoft Visual C++**: Uses inline `__asm` blocks with Intel syntax
- **GNU Compiler Collection**: Uses `__asm__` with AT&T syntax
- **Fallback**: Plain C implementations for unsupported platforms

### Usage in Program
Assembly functions are used for:
- Incrementing folder and task counts
- Decrementing counts during deletion operations

---

## 4. Core Functionality

### 4.1 List Management
- Create named lists for organizing different task categories
- Switch between lists with numbered selection
- Delete lists with automatic index adjustment
- Current list indicated with '>' marker

### 4.2 Task Management
- Add tasks with descriptions and deadlines
- View tasks sorted by deadline (earliest first)
- Mark tasks as complete
- Delete tasks permanently
- Automatic date validation

### 4.3 Intelligent Sorting System
**Sorting Rules:**
1. Completed tasks always appear at the bottom
2. Incomplete tasks sorted by deadline (earliest first)
3. Automatic sorting after adding or completing tasks

### 4.4 Overdue Detection
- Real-time comparison of deadlines against current system time
- Visual indicator [OVERDUE] for tasks past their deadline
- Only applies to incomplete tasks

---

## 5. Data Persistence

### File Storage Format
Data stored in binary format (`todo_data.dat`):
1. Folder count
2. All folder names and task counts
3. Complete task data for each folder
4. Current folder selection

### Features
- Automatic save on program exit
- Automatic load on program startup
- Manual save/load options available
- Preserves complete program state including selected folder

---

## 6. User Interface

### Menu System (11 Options)

**List Management:**
- Create, view, select, and delete lists

**Task Management:**
- Add, view, complete, and delete tasks

**Data Management:**
- Save, load, and exit operations

### Visual Indicators
- `>` marker shows currently selected list
- `[X]` indicates completed tasks
- `[ ]` indicates incomplete tasks
- `[OVERDUE]` flags tasks past deadline

---

## 7. Technical Specifications

**Programming Language:** C (C99 or later)  
**Assembly Language:** x86 (32-bit)  
**Compilers Supported:** MSVC, GCC  
**Standard Libraries:** stdio.h, string.h, stdlib.h, time.h  
**Data File Format:** Binary (.dat)  
**Code Size:** ~500 lines

**Capacity Limits:**
- Maximum folders: 20
- Maximum tasks per folder: 50
- Maximum description length: 100 characters

**Date Format:** YYYY-MM-DD (ISO 8601 compliant)

---

## 8. Conclusion

The To-Do List Manager successfully demonstrates fundamental programming concepts including structured data types, file I/O, sorting algorithms, and assembly language integration. The program provides practical task management functionality while serving as an educational example of C programming techniques and low-level assembly integration.

**Key Strengths:**
- Clear code organization with single-responsibility functions
- Effective integration of C and assembly language
- User-friendly console interface
- Reliable data persistence
- Intelligent task sorting and overdue detection
