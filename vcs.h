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

// typedef branch *vcs;
typedef struct vcs
{
    branch *B;
    char *current_branch;
} vcs;

void vcs_init(vcs *V);
void vcs_track(vcs *V);
void vcs_commit(vcs *V);
void vcs_revert(vcs *V, int version);
void vcs_branch(vcs *V, char *branch);
void vcs_checkout(vcs *V, char *b);
void vcs_status(vcs *V);