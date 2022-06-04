typedef struct node
{
    char *filename;
    int modified;
    int deleted;
    int tracked;
    struct node *next;
} node;

typedef node *file_list;

typedef struct branch
{
    char *name;
    file_list FL;
    struct branch *next;
    int commit;
} branch;

typedef branch *vcs;

int vcs_init(vcs *v);
int vcs_track(vcs *v, char *branch);
// int vcs_commit(vcs *v, char *branch);