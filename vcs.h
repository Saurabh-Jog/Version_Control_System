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

void vcs_init(vcs *v);
void vcs_track(vcs *v, char *branch);
void vcs_commit(vcs *v, char *branch);
void vcs_revert(vcs *v, char *branch, int version);