#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <math.h>
#include <limits.h>
#include "vcs.h"

int main()
{
    vcs v;
    vcs_init(&v);
    printf("Success\n");
    int a = 1;
    printf("loop on or off: ");
    scanf("%d", &a);
    int b = 0;
    while (a)
    {
        printf("wanna track?: ");
        scanf("%d", &b);
        if (b)
            vcs_track(&v, "master");
        printf("wanna commit?: ");
        scanf("%d", &b);
        if (b)
            vcs_commit(&v, "master");
        printf("wanna go back?: ");
        scanf("%d", &b);
        if (b)
        {
            int c;
            printf("Enter version number: ");
            scanf("%d", &c);
            vcs_revert(&v, "master", c);
            node *p = v->FL;
            while (p)
            {
                printf("filename: %s, tracked: %d, modified: %d, deleted: %d\n", p->filename, p->tracked, p->modified, p->deleted);
                p = p->next;
            }
        }

        printf("Continue loop: ");
        scanf("%d", &a);
    }
    return 0;
}