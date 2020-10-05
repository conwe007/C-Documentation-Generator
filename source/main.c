#include "main.h"

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        fprintf(stderr, "error: incorrect number of arguments, expected 3 and got %i\n", argc);
        return 1;
    }

    project_t* project = project_create();
    project_populate(project, argv[1]);
    project_write(project, argv[2]);
    project_destroy(project);

    return 0;
}
