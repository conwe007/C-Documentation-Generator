#ifndef __PARAMETER_H
#define __PARAMETER_H

#include <stdio.h>
#include <stdlib.h>

#define STR_LENGTH 2048

typedef struct parameter_t
{
    char name[STR_LENGTH];
    char description[STR_LENGTH];
    struct parameter_t* next;
} parameter_t;

/**
 * Creates a parameter node.
 * 
 * @return a new parameter node
 */
parameter_t* parameter_create();

/**
 * Destroys a parameter node, freeing its memory.
 * 
 * @param parameter the parameter to be destroyed
 */
void parameter_destroy(parameter_t* parameter);

#endif /* __PARAMETER_H */
