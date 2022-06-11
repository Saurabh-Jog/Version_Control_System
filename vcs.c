#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include "vcs.h"
#include "file_functions.h"
typedef struct dirent *dir;

void vcs_init(vcs *v)
{
    DIR *dr1 = opendir(".");
    if (!dr1)
        return;
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
    closedir(dr1);

    if (flag)
    {
        *v = (branch *)malloc(sizeof(branch));
        if (!*v)
            return;
        (*v)->name = (char *)malloc(sizeof(char) * 7);
        strcpy((*v)->name, "master");
        (*v)->FL = NULL;
        (*v)->next = NULL;
        (*v)->commit = 0;

        DIR *dr = opendir("../");
        if (!dr)
            return;
        dir sd;
        while ((sd = readdir(dr)) != NULL)
        {
            if (is_compatible(sd->d_name))
            {
                node *nn = (node *)malloc(sizeof(node));
                if (!nn)
                    return;
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

        system("mkdir .vcs");
        system("mkdir .vcs/master");
        system("mkdir .vcs/master/C0");
        // FILE *fp = fopen(".vcs/master/save.txt", "w");
        // fclose(fp);

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
            free(zeroth_commit_address);
            free(current_file_address);
            p = p->next;
        }
        return;
    }

    DIR *dr = opendir(".vcs");
    if (!dr)
        return;
    dir sd;
    while ((sd = readdir(dr)) != NULL)
    {
        if (sd->d_name[0] != '.')
        {
            char *branch_address = (char *)malloc(sizeof(char) * 30);
            strcpy(branch_address, ".vcs/");
            strcat(branch_address, sd->d_name);
            branch *nb = (branch *)malloc(sizeof(branch));
            nb->name = (char *)malloc(sizeof(char) * 20);
            strcpy(nb->name, sd->d_name);
            nb->next = NULL;
            nb->FL = NULL;
            nb->commit = -1;
            DIR *dr1 = opendir(branch_address);
            if (!dr1)
                return;
            dir sd1;
            while ((sd1 = readdir(dr1)) != NULL)
            {
                if (sd1->d_name[0] != '.')
                    nb->commit++;
            }
            closedir(dr1);
            char *last_commit = (char *)malloc(sizeof(char) * 40);
            strcpy(last_commit, ".vcs/");
            strcat(last_commit, sd->d_name);
            strcat(last_commit, "/C");
            char commit_num[5];
            sprintf(commit_num, "%d", nb->commit);
            strcat(last_commit, commit_num);
            DIR *dr2 = opendir(last_commit);
            if (!dr2)
                return;
            dir sd2;
            while ((sd2 = readdir(dr2)) != NULL)
            {
                if (sd2->d_name[0] != '.')
                {
                    node *nn = (node *)malloc(sizeof(node));
                    nn->filename = (char *)malloc(sizeof(char) * 20);
                    strcpy(nn->filename, sd2->d_name);
                    nn->modified = 0;
                    nn->deleted = 0;
                    nn->tracked = 0;
                    nn->next = NULL;

                    nn->next = nb->FL;
                    nb->FL = nn;
                }
            }
            closedir(dr2);
            nb->next = (*v);
            (*v) = nb;
            free(branch_address);
            free(last_commit);
        }
    }
    closedir(dr);
    return;
}

void vcs_track(vcs *v, char *branch)
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
            return;
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
                return;
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
        return;
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
                    return;
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
    return;
}

void vcs_commit(vcs *v, char *branch)
{
    vcs q;
    q = *v;
    while (q)
    {
        if (!strcmp(branch, q->name))
            break;
        q = q->next;
    }
    q->commit++;

    char commit_address[30];
    strcpy(commit_address, ".vcs/");
    strcat(commit_address, q->name);
    strcat(commit_address, "/C");
    char commit_num[5];
    sprintf(commit_num, "%d", q->commit);
    strcat(commit_address, commit_num);
    char command[60] = "mkdir ";
    strcat(command, commit_address);
    system(command);

    node *p = q->FL;
    while (p)
    {
        if (!p->deleted)
        {
            char *new_version_address = (char *)malloc(sizeof(char) * 50);
            strcpy(new_version_address, commit_address);
            strcat(new_version_address, "/");
            strcat(new_version_address, p->filename);
            char *current_version_address = (char *)malloc(sizeof(char) * 30);
            strcpy(current_version_address, "../");
            strcat(current_version_address, p->filename);
            file_copy(current_version_address, new_version_address);
            p->modified = 0;
            p = p->next;
            free(new_version_address);
            free(current_version_address);
        }
        else
        {
            node *r = p;
            p = p->next;
            node *s = q->FL;

            if (q->FL == r)
            {
                q->FL = r->next;
            }
            else
            {
                while (s->next != r)
                    s = s->next;
                s->next = r->next;
            }
            free(r);
        }
    }
    return;
}

void vcs_revert(vcs *v, char *branch, int version)
{
    vcs q;
    q = *v;
    while (q)
    {
        if (!strcmp(branch, q->name))
            break;
        q = q->next;
    }
    if (version > q->commit || version < 0)
    {
        printf("invalid version id!\n");
        return;
    }

    DIR *dr = opendir("../");
    if (!dr)
        return;
    dir sd;
    char *parent_path = (char *)malloc(sizeof(char) * 3);
    strcpy(parent_path, "../");
    while ((sd = readdir(dr)) != NULL)
    {
        if (is_compatible(sd->d_name))
        {
            char *current_version_path = (char *)malloc(sizeof(char) * 25);
            strcpy(current_version_path, parent_path);
            strcat(current_version_path, sd->d_name);
            remove(current_version_path);
            free(current_version_path);
        }
    }
    closedir(dr);
    free(parent_path);

    while (q->FL)
    {
        node *p = q->FL;
        q->FL = p->next;
        free(p);
    }

    char revert_path[40] = ".vcs/";
    strcat(revert_path, branch);
    strcat(revert_path, "/C");
    char revert_commit_number[5];
    sprintf(revert_commit_number, "%d", version);
    strcat(revert_path, revert_commit_number);
    DIR *dr1 = opendir(revert_path);
    if (!dr1)
        return;
    dir sd1;

    while ((sd1 = readdir(dr1)) != NULL)
    {
        if (is_compatible(sd1->d_name))
        {
            char *parent_path = (char *)malloc(sizeof(char) * 3);
            strcpy(parent_path, "../");
            char *revert_version_path = (char *)malloc(sizeof(char) * 60);
            strcpy(revert_version_path, revert_path);
            strcat(revert_version_path, "/");
            strcat(revert_version_path, sd1->d_name);
            char *current_version_path = (char *)malloc(sizeof(char) * 30);
            strcpy(current_version_path, parent_path);
            strcat(current_version_path, sd1->d_name);
            file_copy(revert_version_path, current_version_path);

            node *nn = (node *)malloc(sizeof(node));
            nn->filename = (char *)malloc(sizeof(char) * strlen(sd1->d_name));
            strcpy(nn->filename, sd1->d_name);
            nn->tracked = 0;
            nn->modified = 0;
            nn->deleted = 0;
            nn->next = q->FL;
            q->FL = nn;
            free(revert_version_path);
            free(current_version_path);
            free(parent_path);
        }
    }
    closedir(dr1);
    return;
}