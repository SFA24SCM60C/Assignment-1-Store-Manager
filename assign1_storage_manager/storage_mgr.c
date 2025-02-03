#include "storage_mgr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Initialize the storage manager */
void initStorageManager(void) {
    printf("Storage Manager initialized.\n");
}

/* Create a new page file */
RC createPageFile(char *fileName) {
    FILE *file = fopen(fileName, "w+b");
    if (file == NULL) {
        return RC_FILE_NOT_FOUND;
    }

    // Allocate one page of PAGE_SIZE and initialize with '\0'
    SM_PageHandle emptyPage = (SM_PageHandle)calloc(PAGE_SIZE, sizeof(char));
    fwrite(emptyPage, sizeof(char), PAGE_SIZE, file);
    free(emptyPage);
    fclose(file);

    return RC_OK;
}

/* Open an existing page file */
RC openPageFile(char *fileName, SM_FileHandle *fHandle) {
    FILE *file = fopen(fileName, "r+b");
    if (file == NULL) {
        return RC_FILE_NOT_FOUND;
    }

    fseek(file, 0, SEEK_END);
    int fileSize = ftell(file);
    int totalNumPages = fileSize / PAGE_SIZE;
    rewind(file);

    fHandle->fileName = fileName;
    fHandle->totalNumPages = totalNumPages;
    fHandle->curPagePos = 0;
    fHandle->mgmtInfo = file;

    return RC_OK;
}

/* Close an open page file */
RC closePageFile(SM_FileHandle *fHandle) {
    if (fHandle->mgmtInfo == NULL) {
        return RC_FILE_HANDLE_NOT_INIT;
    }
    fclose((FILE *)fHandle->mgmtInfo);
    fHandle->mgmtInfo = NULL;
    return RC_OK;
}

/* Destroy a page file */
RC destroyPageFile(char *fileName) {
    if (remove(fileName) != 0) {
        return RC_FILE_NOT_FOUND;
    }
    return RC_OK;
}

/* Read block at position pageNum */
RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {
    if (fHandle->mgmtInfo == NULL || pageNum >= fHandle->totalNumPages || pageNum < 0) {
        return RC_READ_NON_EXISTING_PAGE;
    }

    FILE *file = (FILE *)fHandle->mgmtInfo;
    fseek(file, pageNum * PAGE_SIZE, SEEK_SET);
    fread(memPage, sizeof(char), PAGE_SIZE, file);
    fHandle->curPagePos = pageNum;

    return RC_OK;
}

/* Get current block position */
int getBlockPos(SM_FileHandle *fHandle) {
    return fHandle->curPagePos;
}

/* Read first block */
RC readFirstBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    return readBlock(0, fHandle, memPage);
}

/* Read previous block */
RC readPreviousBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    if (fHandle->curPagePos <= 0) {
        return RC_READ_NON_EXISTING_PAGE;
    }
    return readBlock(fHandle->curPagePos - 1, fHandle, memPage);
}

/* Read current block */
RC readCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    return readBlock(fHandle->curPagePos, fHandle, memPage);
}

/* Read next block */
RC readNextBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    if (fHandle->curPagePos >= fHandle->totalNumPages - 1) {
        return RC_READ_NON_EXISTING_PAGE;
    }
    return readBlock(fHandle->curPagePos + 1, fHandle, memPage);
}

/* Read last block */
RC readLastBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    return readBlock(fHandle->totalNumPages - 1, fHandle, memPage);
}

/* Write block at position pageNum */
RC writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {
    if (fHandle->mgmtInfo == NULL || pageNum > fHandle->totalNumPages || pageNum < 0) {
        return RC_WRITE_FAILED;
    }

    FILE *file = (FILE *)fHandle->mgmtInfo;
    fseek(file, pageNum * PAGE_SIZE, SEEK_SET);
    fwrite(memPage, sizeof(char), PAGE_SIZE, file);
    fHandle->curPagePos = pageNum;

    if (pageNum == fHandle->totalNumPages) {
        fHandle->totalNumPages++;
    }

    return RC_OK;
}

/* Write current block */
RC writeCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    return writeBlock(fHandle->curPagePos, fHandle, memPage);
}

/* Append an empty block */
RC appendEmptyBlock(SM_FileHandle *fHandle) {
    FILE *file = (FILE *)fHandle->mgmtInfo;
    fseek(file, 0, SEEK_END);
    SM_PageHandle emptyPage = (SM_PageHandle)calloc(PAGE_SIZE, sizeof(char));
    fwrite(emptyPage, sizeof(char), PAGE_SIZE, file);
    free(emptyPage);
    fHandle->totalNumPages++;
    return RC_OK;
}

/* Ensure capacity of pages */
RC ensureCapacity(int numberOfPages, SM_FileHandle *fHandle) {
    while (fHandle->totalNumPages < numberOfPages) {
        appendEmptyBlock(fHandle);
    }
    return RC_OK;
}
