# Assignment-1-Store-Manager

## Overview
This project implements a simple **Storage Manager** that handles reading and writing fixed-size pages (blocks) to disk files. The Storage Manager supports creating, opening, reading, writing, and deleting page files while maintaining metadata for each open file. The project is designed to demonstrate fundamental file handling operations in C.

## Features
- **Fixed Page Size:** Operates on pages of size 4096 bytes (defined in `dberror.h`).
- **File Operations:**
  - Create, open, close, and delete page files.
  - Track the total number of pages and the current page position in open files.
- **Reading and Writing Operations:**
  - Read and write pages using both absolute and relative addressing.
  - Append new pages and ensure capacity of files.
- **Error Handling:**
  - Robust error management using predefined return codes (`RC`) for common file operation errors.

## Project Structure

```
assign1/
├── dberror.c            # Implements error handling functions
├── dberror.h            # Defines error codes and error handling macros
├── storage_mgr.c        # Implements the Storage Manager functions
├── storage_mgr.h        # Declares the Storage Manager interface and data structures
├── test_assign1_1.c     # Test cases to validate the Storage Manager functionality
├── test_helper.h        # Helper macros for testing
├── Makefile             # Build configuration for compiling and running the project
└── README.txt           # Project description and instructions
```

## Compilation and Running Instructions

### Prerequisites
Ensure you have **GCC** installed on your system. If using Windows, you may need to install **MinGW** or use **WSL**.

### Using `make` (Unix/Linux/WSL/MinGW)
1. **Navigate to the project directory:**
   ```bash
   cd path/to/assign1
   ```
2. **Compile the project:**
   ```bash
   make
   ```
3. **Run the tests:**
   ```bash
   make run
   ```
4. **Clean up compiled files:**
   ```bash
   make clean
   ```

### Without `make` (Windows CMD/PowerShell)
1. **Navigate to the project directory:**
   ```bash
   cd path\to\assign1
   ```
2. **Compile manually using GCC:**
   ```bash
   gcc -Wall -o test_assign1 dberror.c storage_mgr.c test_assign1_1.c
   ```
3. **Run the executable:**
   ```bash
   ./test_assign1
   ```

## Expected Output
Upon running the tests, you should see messages indicating the success of various file operations. Example output:

```
[test_assign1_1.c-test create open and close methods-L35-12:00:00] OK: expected true: filename correct
[test_assign1_1.c-test single page content-L67-12:00:00] OK: expected true: expected zero byte in first page of freshly initialized page
...
[test_assign1_1.c-test single page content-L85-12:00:00] OK: finished test
```

## Code Overview

### Data Structures
- **SM_FileHandle:** Represents an open page file with metadata.
  ```c
  typedef struct SM_FileHandle {
      char *fileName;      // Name of the file
      int totalNumPages;   // Total number of pages in the file
      int curPagePos;      // Current page position
      void *mgmtInfo;      // Management info (e.g., FILE pointer)
  } SM_FileHandle;
  ```

- **SM_PageHandle:** Pointer to a block of memory for reading/writing pages.
  ```c
  typedef char* SM_PageHandle;
  ```

### Key Functions
- **File Management:**
  - `initStorageManager()`: Initializes the storage manager.
  - `createPageFile(char *fileName)`: Creates a new page file initialized with zero bytes.
  - `openPageFile(char *fileName, SM_FileHandle *fHandle)`: Opens an existing page file.
  - `closePageFile(SM_FileHandle *fHandle)`: Closes an open page file.
  - `destroyPageFile(char *fileName)`: Deletes a page file.

- **Reading Blocks:**
  - `readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)`: Reads a block at the specified position.
  - `readFirstBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)`: Reads the first block of the file.
  - `readCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)`: Reads the current block based on `curPagePos`.

- **Writing Blocks:**
  - `writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)`: Writes a block at the specified position.
  - `appendEmptyBlock(SM_FileHandle *fHandle)`: Appends an empty block filled with zero bytes.
  - `ensureCapacity(int numberOfPages, SM_FileHandle *fHandle)`: Ensures the file has at least the specified number of pages.

## Error Handling
The project uses standardized return codes (defined in `dberror.h`) to handle errors.

### Common Return Codes
- `RC_OK`: Operation successful.
- `RC_FILE_NOT_FOUND`: The specified file was not found.
- `RC_FILE_HANDLE_NOT_INIT`: File handle not initialized.
- `RC_WRITE_FAILED`: Writing to the file failed.
- `RC_READ_NON_EXISTING_PAGE`: Attempted to read a non-existing page.

Use the `printError(RC)` function to output descriptive error messages when an error occurs.


