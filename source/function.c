#include "function.h"

/**
 * Creates a function structure.
 * 
 * @return a function structure
 */
function_t* function_create()
{
    function_t* function = (function_t*)malloc(sizeof(function_t));

    function->title[0] = '\0';
    function->function_declaration[0] = '\0';
    function->function_description[0] = '\0';

    function->parameter = NULL;

    function->return_description[0] = '\0';

    function->next = NULL;

    return function;
}

/**
 * Destroys a function structure and frees its memory.
 * 
 * @param function the function to be destroyed
 */
void function_destroy(function_t* function)
{
    if(function == NULL)
    {
        return;
    }

    function_destroy(function->next);

    parameter_destroy(function->parameter);

    free(function);
}

/**
 * Populates a function structure based on the contents of the supplied file.
 * 
 * @param function the function structure to populate
 * @param fp a pointer to the file to parse
 * @return a populated function structure
 */
function_t* function_populate(function_t* function, FILE* fp)
{
    if(fp == NULL)
    {
        fprintf(stderr, "error: file pointer is NULL in function_populate()\n");
        return NULL;
    }

    char buffer[STR_LENGTH];

    fgets(buffer, STR_LENGTH, fp);

    // search for the beginning of the javadoc comment
    while(buffer[0] != '/' || buffer[1] != '*' || buffer[2] != '*')
    {
        fgets(buffer, STR_LENGTH, fp);

        if(feof(fp))
        {
            // no more javadoc comments found, leave the function
            return NULL;
        }
    }

    // pull the function description, offset the beginning of the buffer by 3 to trim the leading spaces and *
    fgets(buffer, STR_LENGTH, fp);
    strcpy(function->function_description, buffer + sizeof(char) * 3);

    // remove trailing whitespace
    function->function_description[strlen(function->function_description) - 1] = '\0';

    fgets(buffer, STR_LENGTH, fp);

    // skip the empty line
    if(buffer[0] != ' ' || buffer[1] != '*' || buffer[2] != '/')
    {
        fgets(buffer, STR_LENGTH, fp);
    }
    
    parameter_t* ptr_parameter;
    char* token = NULL;
    char buffer_temp[STR_LENGTH];

    // loop through the parameters
    while(buffer[3] == '@' && buffer[4] == 'p')
    {
        if(function->parameter == NULL)
        {
            function->parameter = parameter_create();
            ptr_parameter = function->parameter;
        }
        else
        {
            ptr_parameter = function->parameter;

            // loop to the end of the parameter list
            while(ptr_parameter->next != NULL)
            {
                ptr_parameter = ptr_parameter->next;
            }

            ptr_parameter->next = parameter_create();
            ptr_parameter = ptr_parameter->next;
        }

        strcpy(buffer_temp, buffer);

        // tokenize and get rid of "*" and "@param"
        token = strtok(buffer_temp, " ");
        token = strtok(NULL, " ");
        token = strtok(NULL, " ");

        // the first word is the name of the parameter
        strcpy(ptr_parameter->name, token);
        token = strtok(NULL, " ");

        // the next words are the parameter description
        while(token != NULL)
        {
            strcat(ptr_parameter->description, token);
            strcat(ptr_parameter->description, " ");
            token = strtok(NULL, " ");
        }

        // remove trailing whitespace
        ptr_parameter->description[strlen(ptr_parameter->description) - 2] = '\0';
        
        fgets(buffer, STR_LENGTH, fp);
    }

    // loop to the end of the javadoc comment
    while(buffer[0] != ' ' || buffer[1] != '*' || buffer[2] != '/')
    {
        if(buffer[3] == '@' && buffer[4] == 'r')
        {
            // pull the function description, offset the beginning of the buffer by 11 to trim the leading * @return
            strcpy(function->return_description, buffer + sizeof(char) * 11);

            // remove trailing whitespace
            function->return_description[strlen(function->return_description) - 1] = '\0';
        }

        fgets(buffer, STR_LENGTH, fp);
    }

    fgets(buffer, STR_LENGTH, fp);
    
    // next line is function declaration, remove trailing ;
    snprintf(function->function_declaration, strlen(buffer) - 1, "%s", buffer);

    // tokenize the function declaration to find the name
    strcpy(buffer_temp, buffer);
    token = strtok(buffer_temp, " (");
    token = strtok(NULL, " (");
    strcpy(function->title, token);
    strcat(function->title, "()");

    return function;
}

/**
 * Writes a string of generated html to the supplied file.
 * 
 * @param function the function to generate html from
 * @param fp a pointer to the file to write to
 */
void function_write(function_t* function, FILE* fp)
{
    if(function == NULL)
    {
        fprintf(stderr, "error: null function pointer supplied to function_write()\n");
        return;
    }

    if(fp == NULL)
    {
        fprintf(stderr, "error: null file pointer supplied to function_write()\n");
        return;
    }

    function_t* ptr_function = function;

    fprintf(fp, "<div class='function'>\n");

    fprintf(fp, "\t<div class='function-header'>\n");
    fprintf(fp, "\t\t<h1 class='function-title'>%s</h1>\n", ptr_function->title);
    fprintf(fp, "\t</div>\n");

    fprintf(fp, "\t<div class='function-body'>\n");

    fprintf(fp, "\t\t<h2 class='function-declaration'>%s</h2>\n", ptr_function->function_declaration);
    fprintf(fp, "\t\t<span class='function-description'>%s</span>\n", ptr_function->function_description);
    
    parameter_t* ptr_parameter = ptr_function->parameter;
    
    if(ptr_parameter != NULL)
    {
        fprintf(fp, "\t\t<h3 class='parameter'>Parameters</h3>\n");
        while(ptr_parameter != NULL)
        {
            fprintf(fp, "\t\t<span class='parameter-title'>%s</span>\n", ptr_parameter->name);
            fprintf(fp, "\t\t<span class='parameter-description'>%s</span>\n", ptr_parameter->description);
            fprintf(fp, "\t\t<br/>\n");
            ptr_parameter = ptr_parameter->next;
        }
    }

    if(ptr_function->return_description[0] != '\0')
    {
        fprintf(fp, "\t\t<h3 class='return'>Returns</h3>\n");
        fprintf(fp, "\t\t<span class='return-description'>%s</span>\n", ptr_function->return_description);
    }

    fprintf(fp, "\t</div>\n");

    fprintf(fp, "</div>\n");
}
