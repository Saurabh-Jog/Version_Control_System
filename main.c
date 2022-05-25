#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <math.h>
#include <limits.h>
#include "vcs.h"
#include "fileCommands.h"

int main()
{
    fileCopy("file1.txt", "file2.txt");
    printf("Success!");
}