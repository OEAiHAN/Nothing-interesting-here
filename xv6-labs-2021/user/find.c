#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char* name)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s.\n", path);
        return;
    }

    if(fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s.\n", path);
        close(fd);
        return;
    }
    
    if (st.type != T_DIR)
    {
    	fprintf(2, "find: the first pattern must be a directory.\n");
    	return;
    }

    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
	fprintf(2, "find: path too long.\n");
	return;
    }
        
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if(de.inum == 0)
	    continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0)
        {
            fprintf(2, "find: cannot stat %s.\n", buf);
            continue;
        }
        
        switch (st.type)
        {
        case T_DIR:
            if (strcmp(p, ".") && strcmp(p, ".."))
                find(buf, name);
            break;
        default: 
            if (!strcmp(p, name))
                printf("%s\n", buf);
            break;
        }
    }
    
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
    	printf("find <directory> <filename>\n");
    	exit(1);
    }
    else if (argc != 3)
    {
    	printf("find: wrong patterns count.\n");
    	exit(1);
    }
    
    find(argv[1], argv[2]);
    exit(0);
    
}
