#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define RD 0
#define WR 1

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
    	printf("pingpong: too many patterns.\n");
    	exit(1);
    }

    char buf[16];
    char ping = 'P';
    int l[2], r[2];
    
    pipe(l);
    pipe(r);
    
    int x = fork();
    if (x < 0)
    {
    	printf("pingpong: fork error!\n");
    	close(l[RD]);
    	close(l[WR]);
    	close(r[RD]);
    	close(r[WR]);
    	exit(1);
    }
    else if (!x)
    {
    	close(l[WR]);
    	close(r[RD]);
    
    	if (read(l[RD], buf, sizeof(char)) < 0)
    	{
    	    printf("pingpong: child process read error.\n");
    	    exit(1);
    	}
    	else
    	    printf("<%d>: received ping\n", getpid());
    	
    	if (write(r[WR], &ping, sizeof(char)) < 0)
    	{
    	    printf("pingpong: child process write error.\n");
    	    exit(1);
    	}
    	    
    	close(l[RD]);
    	close(r[WR]);
    	exit(0);
    	
    }
    else
    {
    	close(l[RD]);
    	close(r[WR]);
    
    	if (write(l[WR], &ping, sizeof(char)) < 0)
    	{
    	    printf("pingpong: parent process write error.\n");
    	    exit(1);
    	}
    	
    	if (read(r[RD], buf, sizeof(char)) < 0)
    	{
    	    printf("pingpong: parent process read error.\n");
    	    exit(1);
    	}
    	else
    	    printf("<%d>: received pong\n", getpid());
    	    
    	close(l[WR]);
    	close(r[RD]);
    	exit(0);
    }
    
    return 0;
}
