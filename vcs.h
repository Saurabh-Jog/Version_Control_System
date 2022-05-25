#define branch_name_size 20
#define secret_code "1@3()7@3^8*_"

typedef struct
{
    int start;
    int end;
    int is_changed;
    int is_deleted;
} version_info;

typedef struct
{
    char *filename;
    version_info *V;
    struct versions *next;
    struct versions *previous;
} versions;

typedef struct
{
    char name[branch_name_size];
    versions *Ver;
    int V_size;
} branch;

typedef branch *vcs;

int vcs_init(vcs *v);
int vcs_add_file(char *filename, char *branch);