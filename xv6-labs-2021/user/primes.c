#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define RD 0
#define WR 1

void bfs(int f[2], int cp)
{
    printf("primes: %d\n", cp);
    
    int cur, nxt, p[2], flag = 0;
    pipe(p);
    
    while (read(f[RD], &cur, sizeof(int)) == sizeof(int))
    	if (cur % cp)
    	{
    	    if (!flag)
    	    {
    	    	nxt = cur;
    	    	flag = 1;
    	    }
    	    else
    	    	write(p[WR], &cur, sizeof(int));
    	}
    close(f[RD]);
    close(p[WR]);    	
    
    if (flag && !fork())
        bfs(p, nxt);
    if (!flag)
    	close(p[RD]);
    
    wait(0);
    exit(0);
}

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
    	printf("primes -n\n");
    	printf("    -n    bound to search prime numbers.\n");
    	exit(1);
    }
    else if (argc > 2)
    {
    	printf("primes: too many patterns.\n");
    	exit(1);
    }
    else if (atoi(argv[1]) <= 1)
    {
    	printf("primes: n should be greater than 1.\n");
    	exit(0);
    }
    
    int p[2];
    pipe(p);
    
    int n = atoi(argv[1]);
    for (int i = 3; i <= n; i++)
    	write(p[WR], &i, sizeof(int));
    close(p[WR]);
    
    bfs(p, 2);
    
    wait(0);
    return 0;
}
