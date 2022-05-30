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
    // printf("%d\n", vcs_init(&v));
    vcs_init(&v);

    while (1)
    {
        int a;
        printf("Enter a value: ");
        scanf("%d", &a);
        vcs_track(&v, "master");
        printf("hehe\n");
        node *p = v->FL;
        while (p)
        {
            printf("%d\n", p->deleted);
            p = p->next;
        }
    }
    // printf("%d\n", is_compatible("hello.tx"));
}