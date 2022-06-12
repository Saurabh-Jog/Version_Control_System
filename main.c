#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <math.h>
#include <limits.h>
#include "vcs.h"

int main()
{
    vcs V;
    vcs_init(&V);
    printf("Success\n");
    int a = 1;
    printf("loop on or off: ");
    scanf("%d", &a);
    int b = 0;
    while (a)
    {
        printf("%s\n", V.current_branch);
        // branch *q = V.B;
        // while (q)
        // {
        //     if (!strcmp(q->name, V.current_branch))
        //         break;
        //     q = q->next;
        // }

        // node *p = q->FL;
        // while (p)
        // {
        //     printf("filename: %s, tracked: %d, modified: %d, deleted: %d\n", p->filename, p->tracked, p->modified, p->deleted);
        //     p = p->next;
        // }
        // printf("\n");
        printf("\n");
        printf("wanna check status?: ");
        scanf("%d", &b);
        if (b)
            vcs_status(&V);

        printf("wanna track?: ");
        scanf("%d", &b);
        if (b)
            vcs_track(&V);

        printf("wanna commit?: ");
        scanf("%d", &b);
        if (b)
            vcs_commit(&V);

        printf("wanna go back?: ");
        scanf("%d", &b);
        if (b)
        {
            int c;
            printf("Enter version number: ");
            scanf("%d", &c);
            vcs_revert(&V, c);
            // node *p = V.B->FL;
            // while (p)
            // {
            //     printf("filename: %s, tracked: %d, modified: %d, deleted: %d\n", p->filename, p->tracked, p->modified, p->deleted);
            //     p = p->next;
            // }
        }

        printf("branch?: ");
        scanf("%d", &b);
        if (b)
        {
            char branch[20];
            printf("Enter branch name: ");
            scanf("%s", branch);
            vcs_branch(&V, branch);
        }

        printf("Wanna change branch?: ");
        scanf("%d", &b);
        if (b)
        {
            char branch[20];
            printf("Enter branch name: ");
            scanf("%s", branch);
            vcs_checkout(&V, branch);
        }

        printf("Continue loop: ");
        scanf("%d", &a);
    }
    return 0;
}