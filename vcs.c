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
    (*v)->commit = 0;

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
    }
    closedir(dr);

    DIR *dr1 = opendir(".");
    dir sd1;
    int flag = 1;
    while ((sd1 = readdir(dr1)) != NULL)
    {
        if (!strcmp(".vcs", sd1->d_name))
        {
            flag = 0;
            break;
        }
    }
    if (flag)
    {
        system("mkdir .vcs");
        system("mkdir .vcs/master");
        system("mkdir .vcs/master/C0");

        node *p = (*v)->FL;
        while (p)
        {
            // char zeroth_commit_address[strlen(p->filename) + 20] = ".vcs/master/C0/";
            char *zeroth_commit_address = (char *)malloc(sizeof(char) * (strlen(p->filename) + 20));
            strcpy(zeroth_commit_address, ".vcs/master/C0/");
            strcat(zeroth_commit_address, p->filename);
            // char current_file_address[strlen(p->filename) + 5] = "../";
            char *current_file_address = (char *)malloc(sizeof(char) * (strlen(p->filename) + 5));
            strcpy(current_file_address, "../");
            strcat(current_file_address, p->filename);
            file_copy(current_file_address, zeroth_commit_address);
            // free(zeroth_commit_address);
            // free(current_file_address);
            p = p->next;
        }
    }
    closedir(dr1);
    return 1;
}

int vcs_track(vcs *v, char *branch)
{
    vcs q;
    q = *v;
    while (q)
    {
        if (!strcmp(branch, q->name))
            break;
        q = q->next;
    }
    node *p = q->FL;
    while (p)
    {
        p->tracked = 1;
        DIR *dr1 = opendir("../");
        if (!dr1)
            return 0;
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

        if (!p->modified)
        {
            // char previous_commit_address[strlen(branch) + 22] = ".vcs/";
            char *previous_commit_address = (char *)malloc(sizeof(char) * (strlen(branch) + 22));
            strcpy(previous_commit_address, ".vcs/");
            char previous_commit_num[5];
            sprintf(previous_commit_num, "%d", q->commit);
            strcat(previous_commit_address, branch);
            strcat(previous_commit_address, "/C");
            strcat(previous_commit_address, previous_commit_num);

            DIR *dr2 = opendir(previous_commit_address);
            if (!dr2)
                return 0;
            dir sd2;
            while ((sd2 = readdir(dr2)) != NULL)
            {
                if (!strcmp(p->filename, sd2->d_name))
                {
                    // char previous_version_address[strlen(previous_commit_address) + strlen(p->filename) + 5];
                    char *previous_version_address = (char *)malloc(sizeof(char) * (strlen(previous_commit_address) + strlen(p->filename) + 5));
                    strcpy(previous_version_address, previous_commit_address);
                    strcat(previous_version_address, "/");
                    strcat(previous_version_address, p->filename);
                    // char current_version_address[strlen(p->filename) + 5] = "../";
                    char *current_version_address = (char *)malloc(sizeof(char) * (strlen(p->filename) + 5));
                    strcpy(current_version_address, "../");
                    strcat(current_version_address, p->filename);
                    if (is_diff(previous_version_address, current_version_address))
                    {
                        // printf("I am is_diff\n");
                        p->modified = 1;
                    }
                    free(previous_version_address);
                    free(current_version_address);
                    break;
                }
            }
            closedir(dr2);
            free(previous_commit_address);
        }
        p = p->next;
    }
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
                nn->modified = 1;
                nn->tracked = 1;
                nn->next = q->FL;
                q->FL = nn;
            }
        }
    }
    closedir(dr);
    return 1;
}

// int vcs_commit(vcs *v, char *branch)
// {

// }