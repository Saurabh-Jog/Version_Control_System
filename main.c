#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <math.h>
#include <limits.h>
#include "vcs.h"
#include "colour.h"

int main()
{
    yellow();
    vcs V;
    vcs_init(&V);
    // int a = 1;
    // printf("loop on or off: ");
    // scanf("%d", &a);
    // int b = 0;
    // while (a)
    // {
    //     printf("%s\n", V.current_branch);
    //     // branch *q = V.B;
    //     // while (q)
    //     // {
    //     //     if (!strcmp(q->name, V.current_branch))
    //     //         break;
    //     //     q = q->next;
    //     // }

    //     // node *p = q->FL;
    //     // while (p)
    //     // {
    //     //     printf("filename: %s, tracked: %d, modified: %d, deleted: %d\n", p->filename, p->tracked, p->modified, p->deleted);
    //     //     p = p->next;
    //     // }
    //     // printf("\n");
    //     printf("\n");
    //     printf("wanna check status?: ");
    //     scanf("%d", &b);
    //     if (b)
    //         vcs_status(&V);

    //     printf("wanna track?: ");
    //     scanf("%d", &b);
    //     if (b)
    //         vcs_track(&V);

    //     printf("wanna commit?: ");
    //     scanf("%d", &b);

    //     if (b)
    //     {
    //         char message[100];
    //         printf("Enter commit message: ");
    //         char c;
    //         scanf("%c", &c);
    //         fgets(message, 100, stdin);
    //         vcs_commit(&V, message);
    //     }

    //     printf("wanna check log?: ");
    //     scanf("%d", &b);
    //     if (b)
    //         vcs_log(&V);

    //     printf("wanna go back?: ");
    //     scanf("%d", &b);
    //     if (b)
    //     {
    //         int c;
    //         printf("Enter version number: ");
    //         scanf("%d", &c);
    //         vcs_revert(&V, c);
    //         // node *p = V.B->FL;
    //         // while (p)
    //         // {
    //         //     printf("filename: %s, tracked: %d, modified: %d, deleted: %d\n", p->filename, p->tracked, p->modified, p->deleted);
    //         //     p = p->next;
    //         // }
    //     }

    //     printf("branch?: ");
    //     scanf("%d", &b);
    //     if (b)
    //     {
    //         char branch[20];
    //         printf("Enter branch name: ");
    //         scanf("%s", branch);
    //         vcs_branch(&V, branch);
    //     }

    //     printf("Wanna change branch?: ");
    //     scanf("%d", &b);
    //     if (b)
    //     {
    //         char branch[20];
    //         printf("Enter branch name: ");
    //         scanf("%s", branch);
    //         vcs_checkout(&V, branch);
    //     }

    //     printf("Continue loop: ");
    //     scanf("%d", &a);
    // }

    int is_on = 1;
    char branch[20];
    char message[100];
    char c;
    int d;

    while (is_on)
    {
        printf("1. vcs_status: See untracked changes\n");
        printf("2. vcs_track: Track changes in the files\n");
        printf("3. vcs_commit: Commit changes\n");
        printf("4. vcs_log: Check commit history\n");
        printf("5. vcs_revert: Return to any previous commit\n");
        printf("6. vcs_branch: Create new branch\n");
        printf("7. vcs_checkout: change active branch\n");
        printf("0. Exit\n\n");

        printf("On branch ");
        cyan();
        printf("%s:\n", get_current_branch(&V));
        yellow();
        int input;
        printf("User input: ");
        scanf("%d", &input);
        printf("\n");

        switch (input)
        {
        case 1:
            vcs_status(&V);
            printf("\n");
            break;

        case 2:
            vcs_track(&V);
            break;

        case 3:
            printf("Enter commit message: ");
            scanf("%c", &c);
            fgets(message, 100, stdin);
            vcs_commit(&V, message);
            break;

        case 4:
            vcs_log(&V);
            printf("\n");
            break;

        case 5:
            printf("Enter version number: ");
            scanf("%d", &d);
            vcs_revert(&V, d);
            printf("\n");
            break;

        case 6:
            printf("Enter branch name: ");
            scanf("%s", branch);
            vcs_branch(&V, branch);
            break;

        case 7:
            printf("Enter branch name: ");
            scanf("%s", branch);
            vcs_checkout(&V, branch);
            break;

        case 0:
            is_on = 0;
            break;

        default:
            red();
            printf("Invalid input\n");
            printf("\n");
            yellow();
            break;
        }
    }

    return 0;
}