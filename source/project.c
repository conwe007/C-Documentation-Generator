#include "project.h"

/**
 * Creates a project structure.
 * 
 * @return a pointer to a new project structure
 */
project_t* project_create()
{
    project_t* project = (project_t*)malloc(sizeof(project_t));

    strcpy(project->project_name, "\0");
    project->file = NULL;

    return project;
}

/**
 * Destroys a project structure, freeing its space in memory.
 * 
 * @param project the project structure to destroy
 */
void project_destroy(project_t* project)
{
    if(project == NULL)
    {
        fprintf(stderr, "error: null pointer supplied to project_destroy()\n");
        return;
    }

    file_destroy(project->file);

    free(project);
}

/**
 * Populates a project struct with data for all files within the specified folder
 * 
 * @param project the project struct to populate
 * @param folder_path the name of the folder to populate from
 * @return a pointer to the supplied project struct
 */
project_t* project_populate(project_t* project, char const* folder_path)
{
    DIR* fd = opendir(folder_path);

    if(fd == NULL)
    {
        fprintf(stderr, "error: failed to open directory '%s' in project_populate()\n", folder_path);
        return NULL;
    }
    
    // keep track of the directory you start in
    char starting_dir[STR_LENGTH];
    getcwd(starting_dir, STR_LENGTH);

    // change working directory to folder you are reading from
    chdir(folder_path);
    
    struct dirent* in_file = readdir(fd);

    FILE* fp = NULL;
    file_t* ptr_file = NULL;
    char working_dir[STR_LENGTH];
    char file_path[STR_LENGTH];

    strcpy(working_dir, "\0");
    strcpy(file_path, "\0");

    getcwd(working_dir, STR_LENGTH);

    // loop through the files in the directory
    while(in_file != NULL)
    {
        // skip current and parent directories
        if(strcmp(in_file->d_name, ".") == 0 || strcmp(in_file->d_name, "..") == 0)
        {
            in_file = readdir(fd);
            continue;
        }

        strcpy(file_path, "\0");

        if(strcmp(working_dir, "\0") == 0)
        {
            fprintf(stderr, "error: could not get working directory in project_populate()\n");
        }

        strcat(file_path, working_dir);
        strcat(file_path, "/");
        strcat(file_path, in_file->d_name);

        fp = fopen(file_path, "r");

        if(fp == NULL)
        {
            fprintf(stderr, "error: failed to open file %s in project_populate() - %s\n", file_path, strerror(errno));
            return NULL;
        }

        if(project->file == NULL)
        {
            project->file = file_create();
            ptr_file = project->file;
        }
        else
        {
            ptr_file = project->file;

            // loop to the end of the file list
            while(ptr_file->next != NULL)
            {
                ptr_file = ptr_file->next;
            }

            ptr_file->next = file_create();
            ptr_file = ptr_file->next;
        }

        // TODO: fix janky way of converting .h to .html
        strcpy(ptr_file->file_name, in_file->d_name);
        strcpy(ptr_file->file_path, file_path);
        strcat(ptr_file->file_path, "tml");

        file_populate(ptr_file, fp);
        
        // no more files were found, so destroy the last one space was malloc'ed for
        if(strcmp(ptr_file->file_name, "\0") == 0)
        {
            file_t* ptr_file_curr = project->file;
            file_t* ptr_file_next = ptr_file_curr->next;

            // loop to the end of the parameter list
            while(ptr_file_next->next != NULL)
            {
                ptr_file_curr = ptr_file_next;
                ptr_file_next = ptr_file_next->next;
            }

            file_destroy(ptr_file_next);
            ptr_file_curr->next = NULL;
        }

        fclose(fp);

        in_file = readdir(fd);
    }

    closedir(fd);

    // go back to original directory you started in
    chdir(starting_dir);

    return project;
}

/**
 * Writes the contents of project struct to html files
 * 
 * @param project the project struct to write
 * @param folder_path the path of the folder to write to
 */
void project_write(project_t* project, char const* folder_path)
{
    DIR* fd = opendir(folder_path);

    if(fd == NULL)
    {
        fprintf(stderr, "error: failed to open directory '%s' in project_write()\n", folder_path);
        return;
    }

    // keep track of the directory you start in
    char starting_dir[STR_LENGTH];
    getcwd(starting_dir, STR_LENGTH);
    
    // change working directory to the directory you will be writing to
    chdir(folder_path);

    FILE* fp = NULL;
    char working_dir[STR_LENGTH];
    char file_path[STR_LENGTH];

    strcpy(working_dir, "\0");
    strcpy(file_path, "\0");

    getcwd(working_dir, STR_LENGTH);
    strcat(file_path, working_dir);
    strcat(file_path, "/index.html");

    fp = fopen(file_path, "w");

    if(fp == NULL)
    {
        fprintf(stderr, "error: failed to open file '%s' in project_write()\n", file_path);
        return;
    }

    // boilerplate header data
    fprintf(fp, "<!doctype html>\n\n");
    fprintf(fp, "<html lang='en'>\n\n");
    fprintf(fp, "<head>\n");
    fprintf(fp, "\t<meta charset='utf-8'>\n");
    fprintf(fp, "\t<title>%s</title>\n", "Home");
    fprintf(fp, "\t<link rel='stylesheet' href='style.css'>\n");
    fprintf(fp, "\t<link href='https://fonts.googleapis.com/css2?family=Open+Sans&display=swap' rel='stylesheet'>\n");
    fprintf(fp, "</head>\n\n");

    fprintf(fp, "<body>\n\n");

    project_write_navbar(project, fp);

    fprintf(fp, "</body>\n\n");

    fclose(fp);

    file_t* ptr_file = project->file;

    while(ptr_file != NULL)
    {
        // TODO: fix janky way of converting .h to .html
        strcpy(file_path, "\0");
        strcat(file_path, working_dir);
        strcat(file_path, "/");
        strcat(file_path, ptr_file->file_name);
        strcat(file_path, "tml");
    
        fp = fopen(file_path, "w");

        if(fp == NULL)
        {
            fprintf(stderr, "error: failed to open file '%s' in project_write()\n", file_path);
            ptr_file = ptr_file->next;
            continue;
        }

        // boilerplate header data
        fprintf(fp, "<!doctype html>\n\n");
        fprintf(fp, "<html lang='en'>\n\n");
        fprintf(fp, "<head>\n");
        fprintf(fp, "\t<meta charset='utf-8'>\n");
        fprintf(fp, "\t<title>%s</title>\n", ptr_file->file_name);
        fprintf(fp, "\t<link rel='stylesheet' href='style.css'>\n");
        fprintf(fp, "\t<link href='https://fonts.googleapis.com/css2?family=Open+Sans&display=swap' rel='stylesheet'>\n");
        fprintf(fp, "</head>\n\n");

        fprintf(fp, "<body>\n\n");

        project_write_navbar(project, fp);

        file_write(ptr_file, fp);

        fprintf(fp, "</body>\n\n");

        fclose(fp);

        ptr_file = ptr_file->next;
    }

    // go back to original directory you started in
    chdir(starting_dir);
}

/**
 * Writes an html navbar containing links to all files in the project to the file pointed to by fp
 * 
 * @param project the project to get navigation data from
 * @param fp the file to write to
 */
void project_write_navbar(project_t* project, FILE* fp)
{
    fprintf(fp, "<nav>\n");
    fprintf(fp, "\t<ul>\n");

    fprintf(fp, "\t\t<li><a href='index.html'>Home</a></li>\n");

    file_t* ptr_file = project->file;
    char file_name_html[STR_LENGTH];
    
    strcpy(file_name_html, "\0");

    while(ptr_file != NULL)
    {
        strcpy(file_name_html, "\0");
        strcat(file_name_html, ptr_file->file_name);
        strcat(file_name_html, "tml");

        fprintf(fp, "\t\t<li><a href='%s'>%s</a></li>\n", file_name_html, ptr_file->file_name);
    
        ptr_file = ptr_file->next;
    }

    fprintf(fp, "\t</ul>\n");
    fprintf(fp, "</nav>\n\n");
}
