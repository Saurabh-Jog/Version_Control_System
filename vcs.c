#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include "vcs.h"
#include "fileCommands.h"

int vcs_init(vcs *v)
{
    *v = (branch *)malloc(sizeof(branch));
    if (!*v)
        return 0;
    strcpy((*v)->name, "master");
    system("mkdir branches");
    system("mkdir branches/master");
    system("mkdir branches/master/stage");
    system("mkdir branches/master/versions");
    return 1;
}

int vcs_add_file(char *filename, char *branch)
{
    char file_name[max_filename_size + 3] = "../";
    strcat(file_name, filename);
    FILE *fp = fopen(file_name, "r");
    if (!fp)
    {
        printf("Unable to open the file\n");
        return 0;
    }
    fclose(fp);

    char stage_file_name[max_filename_size + 28] = "branches/";
    strcat(stage_file_name, branch);
    strcat(stage_file_name, "/stage/");
    strcat(stage_file_name, filename);
    return fileCopy(file_name, stage_file_name);
}