typedef struct
{
    int start;
    int end;
}
version_info;

typedef struct
{
    char *filename;
    version_info *V;
}versions;

typedef struct
{
    char* name;
    char** storage;
    versions *V;
    int V_size;
}branch;

typedef branch* vcs;