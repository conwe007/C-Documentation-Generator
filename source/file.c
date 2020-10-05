#include "file.h"

/**
 * Creates a file structure.
 * 
 * @return a new file structure
 */
file_t* file_create()
{
    file_t* file = (file_t*)malloc(sizeof(file_t));

    strcpy(file->file_name, "\0");
    strcpy(file->file_path, "\0");
    file->function = NULL;
    file->next = NULL;

    return file;
}

/**
 * Destroys a file structure, freeing its space in memory.
 * 
 * @param file the file structure to destroy
 */
void file_destroy(file_t* file)
{
    if(file == NULL)
    {
        return;
    }

    function_destroy(file->function);
    file_destroy(file->next);

    free(file);
}

/**
 * Populates a file struct with data for all functions within the specified file
 * 
 * @param file the file struct to populate
 * @param fp a pointer to the file to populate from
 * @return a pointer to the supplied file struct
 */
file_t* file_populate(file_t* file, FILE* fp)
{
    function_t* ptr_function = NULL;

    while(!feof(fp))
    {
        if(file->function == NULL)
        {
            file->function = function_create();
            ptr_function = file->function;
        }
        else
        {
            ptr_function = file->function;

            // loop to the end of the parameter list
            while(ptr_function->next != NULL)
            {
                ptr_function = ptr_function->next;
            }

            ptr_function->next = function_create();
            ptr_function = ptr_function->next;
        }

        function_populate(ptr_function, fp);

        // no more functions were found, so destroy the last one space was malloc'ed for
        if(strcmp(ptr_function->title, "\0") == 0)
        {
            function_t* ptr_function_curr = file->function;
            function_t* ptr_function_next = ptr_function_curr->next;

            // loop to the end of the parameter list
            if(ptr_function_next != NULL)
            {
                while(ptr_function_next->next != NULL)
                {
                    ptr_function_curr = ptr_function_next;
                    ptr_function_next = ptr_function_next->next;
                }
            }
            
            function_destroy(ptr_function_next);
            ptr_function_curr->next = NULL;
        }
    }

    return file;
}

/**
 * Writes the contents of the supplied file structure to the file pointed to by fp
 * 
 * @param file the file structure to write
 * @param fp the file pointer to write to
 */
void file_write(file_t* file, FILE* fp)
{
    function_t* ptr_function = file->function;

    fprintf(fp, "<div class='file'>\n");
    fprintf(fp, "\t<h1 class='file-name'>%s</h1>\n", file->file_name);
    fprintf(fp, "</div>\n");

    while(ptr_function != NULL)
    {
        function_write(ptr_function, fp);

        ptr_function = ptr_function->next;
    }
}
