#ifndef __FILE_H
#define __FILE_H

#include "function.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct file_t
{
    char file_name[STR_LENGTH];
    char file_path[STR_LENGTH];
    function_t* function;
    struct file_t* next;
} file_t;

/**
 * Creates a file structure.
 * 
 * @return a pointer to a new file structure
 */
file_t* file_create();

/**
 * Destroys a file structure, freeing its space in memory.
 * 
 * @param file the file structure to destroy
 */
void file_destroy(file_t* file);

/**
 * Populates a file struct with data for all functions within the specified file
 * 
 * @param file the file struct to populate
 * @param fp a pointer to the file to populate from
 * @return a pointer to the supplied file struct
 */
file_t* file_populate(file_t* file, FILE* fp);

/**
 * Writes the contents of the supplied file structure to the file pointed to by fp
 * 
 * @param file the file structure to write
 * @param fp the file pointer to write to
 */
void file_write(file_t* file, FILE* fp);

#endif /* __FILE_H */
