#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <math.h>
#include <limits.h>
#include "vcs.h"
#include "fileCommands.h"

int main()
{
    vcs v;
    vcs_init(&v);
    printf("Success\n");
    // vcs_track(&v, "master");
    int a;
    printf("Enter loop on or off: ");
    scanf("%d", &a);
    while (a)
    {
        printf("Enter a value: ");
        scanf("%d", &a);

        vcs_track(&v, "master");
        node *p = v->FL;
        while (p)
        {
            printf("filename: %s, tracked: %d, modified: %d, deleted: %d\n", p->filename, p->tracked, p->modified, p->deleted);
            p = p->next;
        }
    }
}