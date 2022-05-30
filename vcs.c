#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include "vcs.h"
#include "fileCommands.h"
typedef struct dirent *dir;

int vcs_init(vcs *v)
{
    *v = (branch *)malloc(sizeof(branch));
    if (!*v)
        return 0;
    (*v)->name = (char *)malloc(sizeof(char) * 7);
    strcpy((*v)->name, "master");
    (*v)->FL = NULL;
    (*v)->next = NULL;

    DIR *dr = opendir("../");
    if (!dr)
        return 0;
    dir sd;
    while ((sd = readdir(dr)) != NULL)
    {
        if (is_compatible(sd->d_name))
        {
            node *nn = (node *)malloc(sizeof(node));
            if (!nn)
                return 0;
            nn->filename = (char *)malloc(sizeof(char) * strlen(sd->d_name));
            strcpy(nn->filename, sd->d_name);
            nn->deleted = 0;
            nn->modified = 0;
            nn->tracked = 0;
            nn->next = (*v)->FL;
            (*v)->FL = nn;
        }
        // printf("%s\n", sd->d_name);
    }
    // system("mkdir branches");
    // system("mkdir branches/master");
    // system("mkdir branches/master/stage");
    // system("mkdir branches/master/versions");
    closedir(dr);
    return 1;
}

int vcs_track(vcs *v, char *branch)
{
    printf("*");
    vcs q;
    q = *v;
    while (q)
    {
        if (!strcmp(branch, q->name))
            break;
        q = q->next;
    }
    node *p = q->FL;
    printf("*");
    while (p)
    {
        // int size = strlen(p->filename) + 4;
        // char *file_name = (char *)malloc(sizeof(char) * size);
        // strcpy(file_name, "../");
        // strcat(file_name, p->filename);
        // FILE *fp = fopen(file_name, "r");
        p->tracked = 1;
        // if (!fp)
        //     p->deleted = 1;
        // fclose(fp);
        // free(file_name);
        DIR *dr1 = opendir("../");
        dir sd1;
        int flag1 = 1;
        while ((sd1 = readdir(dr1)) != NULL)
        {
            if (!strcmp(p->filename, sd1->d_name))
            {
                flag1 = 0;
                break;
            }
        }
        if (flag1)
            p->deleted = 1;
        closedir(dr1);
        p = p->next;
    }
    printf("*");
    DIR *dr = opendir("../");
    if (!dr)
        return 0;
    dir sd;
    while ((sd = readdir(dr)) != NULL)
    {
        if (is_compatible(sd->d_name))
        {
            node *r = q->FL;
            int flag = 1;
            while (r)
            {
                if (!strcmp(r->filename, sd->d_name))
                {
                    flag = 0;
                    break;
                }
                r = r->next;
            }

            if (flag)
            {
                node *nn = (node *)malloc(sizeof(node));
                if (!nn)
                    return 0;
                nn->filename = (char *)malloc(sizeof(char) * strlen(sd->d_name));
                strcpy(nn->filename, sd->d_name);
                nn->deleted = 0;
                nn->modified = 0;
                nn->tracked = 1;
                nn->next = q->FL;
                q->FL = nn;
            }
        }
    }
    closedir(dr);
    return 1;
}