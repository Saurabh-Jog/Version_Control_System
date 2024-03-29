#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "file_functions.h"

int is_diff(char *filename1, char *filename2)
{
    FILE *fp1 = fopen(filename1, "r");
    if (!fp1)
        return 0;
    FILE *fp2 = fopen(filename2, "r");
    if (!fp2)
        return 0;

    while (!feof(fp1) && !feof(fp2))
    {
        char ch1 = fgetc(fp1);
        char ch2 = fgetc(fp2);
        if (ch1 != ch2)
            return 1;
    }

    if (feof(fp1) ^ feof(fp2))
        return 1;

    return 0;
}

// Copy contents of source file into destination file(removes previous contents of destination file)
int file_copy(char *source, char *destination)
{
    FILE *fp1 = fopen(source, "r");
    if (!fp1)
        return 0;
    FILE *fp2 = fopen(destination, "w");
    if (!fp2)
        return 0;
    // fprintf(fp2, "\0");
    fp2 = fopen(destination, "a");
    char ch = fgetc(fp1);
    while (ch != EOF)
    {
        fputc(ch, fp2);
        ch = fgetc(fp1);
    }
    fclose(fp1);
    fclose(fp2);
    return 1;
}

int is_compatible(char name[])
{
    int size = strlen(name);
    if (!strcmp(name + size - 2, ".c") || !strcmp(name + size - 2, ".h") || !strcmp(name + size - 4, ".txt"))
    {
        return 1;
    }
    return 0;
}