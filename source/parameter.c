#include "parameter.h"

/**
 * Creates a parameter node.
 * 
 * @return a new parameter node
 */
parameter_t* parameter_create()
{
    parameter_t* parameter = (parameter_t*)malloc(sizeof(parameter_t));

    parameter->name[0] = '\0';
    parameter->description[0] = '\0';
    parameter->next = NULL;

    return parameter;
}

/**
 * Destroys a parameter node, freeing its memory.
 * 
 * @param parameter the parameter to be destroyed
 */
void parameter_destroy(parameter_t* parameter)
{
    if(parameter == NULL)
    {
        return;
    }
    
    parameter_destroy(parameter->next);

    free(parameter);
}
