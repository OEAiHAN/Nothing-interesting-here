#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
    	printf("sleep: too many parameters.\n");
    	exit(1);
    }
    else if (argc < 2)
    {
    	printf("sleep -n\n");
    	printf("    -n    length to sleep.\n");
    	exit(1);
    }
    
    int t = atoi(argv[1]);
    if (t < 0)
    {
    	printf("sleep: the parameter n should be a non-negative number.\n");
    	exit(1);
    }
    
    sleep(t);
    exit(1);
}
