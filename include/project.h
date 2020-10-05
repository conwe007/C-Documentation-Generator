#ifndef __PROJECT_H
#define __PROJECT_H

#include "file.h"
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define INDEX "./docs/index.html"

typedef struct project_t
{
    char project_name[STR_LENGTH];
    file_t* file;
} project_t;

/**
 * Creates a project structure.
 * 
 * @return a pointer to a new project structure
 */
project_t* project_create();

/**
 * Destroys a project structure, freeing its space in memory.
 * 
 * @param project the project structure to destroy
 */
void project_destroy(project_t* project);

/**
 * Populates a project struct with data for all files within the specified folder
 * 
 * @param project the project struct to populate
 * @param folder_path the name of the folder to populate from
 * @return a pointer to the supplied project struct
 */
project_t* project_populate(project_t* project, char const* folder_path);

/**
 * Writes the contents of project struct to html files
 * 
 * @param project the project struct to write
 * @param folder_path the path of the folder to write to
 */
void project_write(project_t* project, char const* folder_path);

/**
 * Writes an html navbar containing links to all files in the project to the file pointed to by fp
 * 
 * @param project the project to get navigation data from
 * @param fp the file to write to
 */
void project_write_navbar(project_t* project, FILE* fp);

#endif /* __PROJECT_H */
