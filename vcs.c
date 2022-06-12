#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include "vcs.h"
#include "file_functions.h"
typedef struct dirent *dir;

void vcs_init(vcs *V)
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
        V->current_branch = (char *)malloc(sizeof(char) * 20);
        strcpy(V->current_branch, "master");
        V->B = (branch *)malloc(sizeof(branch));
        branch *v = V->B;
        v->name = (char *)malloc(sizeof(char) * 20);
        strcpy(v->name, "master");
        v->FL = NULL;
        v->next = NULL;
        v->commit = 0;

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
                nn->next = v->FL;
                v->FL = nn;
            }
        }
        closedir(dr);

        system("mkdir .vcs");
        system("mkdir .vcs/master");
        system("mkdir .vcs/master/C0");
        FILE *fp = fopen(".vcs/master/log.txt", "w");
        fclose(fp);

        node *p = v->FL;
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

    V->current_branch = (char *)malloc(sizeof(char) * 20);
    strcpy(V->current_branch, "master");
    V->B = NULL;
    branch *v = V->B;
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

            nb->next = V->B;
            V->B = nb;
            free(branch_address);
            free(last_commit);
        }
    }
    closedir(dr);
    return;
}

void vcs_track(vcs *V)
{
    branch *q = V->B;
    while (q)
    {
        if (!strcmp(V->current_branch, q->name))
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
            char *previous_commit_address = (char *)malloc(sizeof(char) * 50);
            strcpy(previous_commit_address, ".vcs/");
            char previous_commit_num[5];
            sprintf(previous_commit_num, "%d", q->commit);
            strcat(previous_commit_address, q->name);
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
                    char *previous_version_address = (char *)malloc(sizeof(char) * 100);
                    strcpy(previous_version_address, previous_commit_address);
                    strcat(previous_version_address, "/");
                    strcat(previous_version_address, p->filename);
                    char *current_version_address = (char *)malloc(sizeof(char) * 50);
                    strcpy(current_version_address, "../");
                    strcat(current_version_address, p->filename);
                    if (is_diff(previous_version_address, current_version_address))
                    {
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

int vcs_check(vcs *V, int mode)
{
    int all_tracked = 1;
    branch *q = V->B;
    while (q)
    {
        if (!strcmp(q->name, V->current_branch))
            break;
        q = q->next;
    }

    DIR *dr = opendir("../");
    dir sd;
    while ((sd = readdir(dr)) != NULL)
    {
        if (is_compatible(sd->d_name))
        {
            node *p = q->FL;
            int flag = 0;
            while (p)
            {
                if (!strcmp(p->filename, sd->d_name))
                {
                    flag = 1;
                    break;
                }
                p = p->next;
            }
            if (!flag)
            {
                all_tracked = 0;
                if (!mode)
                    return 0;
                printf("%s (untracked)\n", sd->d_name);
            }
        }
    }
    closedir(dr);

    node *p = q->FL;
    while (p)
    {
        if (!p->deleted)
        {
            int flag = 0;
            DIR *dr1 = opendir("../");
            dir sd1;
            while ((sd1 = readdir(dr1)) != NULL)
            {
                if (is_compatible(sd1->d_name) && !strcmp(p->filename, sd1->d_name))
                {
                    flag = 1;
                    break;
                }
            }
            closedir(dr1);
            if (!flag)
            {
                all_tracked = 0;
                if (!mode)
                    return 0;
                printf("%s (deleted)\n", p->filename);
            }
        }
        p = p->next;
    }

    char *previous_commit_address = (char *)malloc(sizeof(char) * 50);
    strcpy(previous_commit_address, ".vcs/");
    char previous_commit_num[5];
    sprintf(previous_commit_num, "%d", q->commit);
    strcat(previous_commit_address, q->name);
    strcat(previous_commit_address, "/C");
    strcat(previous_commit_address, previous_commit_num);

    p = q->FL;
    while (p)
    {
        if (!p->deleted && !p->modified)
        {
            char *previous_version_address = (char *)malloc(sizeof(char) * 100);
            strcpy(previous_version_address, previous_commit_address);
            strcat(previous_version_address, "/");
            strcat(previous_version_address, p->filename);
            char *current_version_address = (char *)malloc(sizeof(char) * 50);
            strcpy(current_version_address, "../");
            strcat(current_version_address, p->filename);
            if (is_diff(previous_version_address, current_version_address))
            {
                all_tracked = 0;
                if (!mode)
                    return 0;
                printf("%s (modified)\n", p->filename);
            }
            free(previous_version_address);
            free(current_version_address);
        }
        p = p->next;
    }
    free(previous_commit_address);

    if (all_tracked)
    {
        if (!mode)
            return 1;
        printf("All changes tracked\n");
    }
    return 1;
}

void vcs_status(vcs *V)
{
    vcs_check(V, 1);
    return;
}

// void add_to_log(vcs *V, char *commit_message)
// {
// }

void vcs_commit(vcs *V)
{
    if (!vcs_check(V, 0))
    {
        printf("Untracked changes detected! Use 'vcs track' to track changes\n");
        return;
    }

    branch *q = V->B;
    while (q)
    {
        if (!strcmp(V->current_branch, q->name))
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

void vcs_revert(vcs *V, int version)
{
    branch *q = V->B;
    while (q)
    {
        if (!strcmp(V->current_branch, q->name))
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
    strcat(revert_path, q->name);
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

void vcs_branch(vcs *V, char *b)
{

    branch *q = V->B;

    while (q)
    {
        if (!strcmp(q->name, b))
        {
            printf("Branch already exists\n");
            return;
        }
        q = q->next;
    }
    branch *nb = (branch *)malloc(sizeof(branch));
    nb->name = (char *)malloc(sizeof(char) * 20);
    strcpy(nb->name, b);
    nb->next = NULL;
    nb->FL = NULL;
    nb->commit = -1;

    q = V->B;
    while (q)
    {
        if (!strcmp(q->name, V->current_branch))
            break;
        q = q->next;
    }

    node *p = q->FL;
    while (p)
    {
        node *nn = (node *)malloc(sizeof(node));
        nn->filename = (char *)malloc(sizeof(20));
        strcpy(nn->filename, p->filename);
        nn->tracked = 0;
        nn->modified = 0;
        nn->deleted = 0;
        nn->next = NULL;

        nn->next = nb->FL;
        nb->FL = nn;
        p = p->next;
    }

    nb->next = V->B;
    V->B = nb;
    strcpy(V->current_branch, b);

    char *new_branch_address = (char *)malloc(sizeof(char) * 30);
    strcpy(new_branch_address, ".vcs/");
    strcat(new_branch_address, b);
    char command[40] = "mkdir ";
    strcat(command, new_branch_address);
    system(command);
    free(new_branch_address);

    vcs_commit(V);
    return;
}

void vcs_checkout(vcs *V, char *b)
{
    strcpy(V->current_branch, b);
    branch *q = V->B;
    while (q)
    {
        if (!strcmp(q->name, b))
            break;
        q = q->next;
    }

    vcs_revert(V, q->commit);
    return;
}