#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <math.h>
#include <limits.h>
#include "vcs.h"
#include "fileCommands.h"

void init_vcs(char *address, vcs *v)
{
    system(strcat("mkdir ", strcat(address, "/vcs")));
    system("mkdir vcs/master");
    system("mkdir vcs/master/storage");
    system("mkdir vcs/master/versions");
}