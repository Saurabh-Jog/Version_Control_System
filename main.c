#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <math.h>
#include <limits.h>
#include "vcs.h"
#include "fileCommands.h"

int main()
{
    // vcs v;
    // vcs_init(&v);
    printf("%d\n", vcs_add_file("hello.txt", "master"));
}