#ifndef __FUNCTION_H
#define __FUNCTION_H

#include "parameter.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_LENGTH 2048

typedef struct function_t
{
    char title[STR_LENGTH];
    char function_declaration[STR_LENGTH];
    char function_description[STR_LENGTH];
    parameter_t* parameter;
    char return_description[STR_LENGTH];
    struct function_t* next;
} function_t;

/**
 * Creates a function structure.
 * 
 * @return a function structure
 */
function_t* function_create();

/**
 * Destroys a function structure and frees its memory.
 * 
 * @param function the function to be destroyed
 */
void function_destroy(function_t* function);

/**
 * Populates a function structure based on the contents of the supplied file.
 * 
 * @param function the function structure to populate
 * @param fp a pointer to the file to parse
 * @return a populated function structure
 */
function_t* function_populate(function_t* function, FILE* fp);

/**
 * Writes a string of generated html to the supplied file.
 * 
 * @param function the function to generate html from
 * @param fp a pointer to the file to write to
 */
void function_write(function_t* function, FILE* fp);

#endif /* __FUNCTION_H */
