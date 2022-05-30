// #define branch_name_size 20
// #define file_name_size 30

// typedef struct
// {
//     int start;
//     int end;
//     int commit;
//     int is_deleted;
// } version_info;

// typedef struct
// {
//     char *filename;
//     version_info *V;
//     struct versions *next;
//     struct versions *previous;
// } versions;

// typedef struct
// {
//     char name[branch_name_size];
//     versions *Ver;
//     int V_size;
// } branch;

// typedef branch *vcs;

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
} branch;

typedef branch *vcs;

int vcs_init(vcs *v);
int vcs_track(vcs *v, char *branch);
// int vcs_commit(vcs *v, char *branch);
