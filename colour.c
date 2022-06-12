#include <stdio.h>
// colours

void red()
{
    printf("\033[1;31m");
}

void yellow()
{
    printf("\033[1;33m");
}

void green()
{
    printf("\033[0;32m");
}

void blue()
{
    printf("\033[0;34m");
}

void purple()
{
    printf("\033[0;35m");
}

void cyan()
{
    printf("\033[0;36m");
}

void reset()
{
    printf("\033[0m");
}
