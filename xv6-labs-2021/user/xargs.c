#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXSIZE 512

#define S_EOF 0
#define S_WAIT 1
#define S_ARG 2
#define S_ENDSPACE 3
#define S_ENDEOLN 4
#define S_EOLN 5

int changeStatus(int sta, char c)
{
    switch (sta)
    {
    case S_WAIT:
    case S_ENDSPACE:
    case S_ENDEOLN:
    case S_EOLN:
    	switch (c)
    	{
    	case ' ':
    	    return S_WAIT;
    	case '\n':
    	    return S_EOLN;
    	default:
    	    return S_ARG;
    	}
    	break;
    case S_ARG:
    	switch (c)
    	{
    	case ' ':
    	    return S_ENDSPACE;
    	case '\n':
    	    return S_ENDEOLN;
    	default:
    	    return S_ARG;
    	}
    	break;
    default:
    	break;
    }
    return S_EOF;
}

int main(int argc, char *argv[])
{
    if (argc > MAXARG)
    {
    	fprintf(2, "xargs: too many arguments.\n");
    	exit(1);
    }
    
    char line[MAXSIZE];
    char *p = line;
    char *xargv[MAXARG] = { 0 };
    int begin = 0, end = 0, cnt = argc - 1, sta = S_WAIT;
    
    for (int i = 1; i < argc; i++)
    	xargv[i - 1] = argv[i];
    	
    while (sta)
    {
    	if (read(0, p, sizeof(char)) != sizeof(char))
    	    sta = S_EOF;
	else    	
            sta = changeStatus(sta, *p);
    		
    	if (++end >= MAXSIZE)
    	{
    	    fprintf(2, "xargs: argument too long.\n");
    	    exit(1);
	} 
    	
    	switch (sta)
    	{
    	case S_WAIT:
    	    begin++;
    	    break;
    	case S_ENDSPACE:
    	    xargv[cnt++] = line + begin;
    	    begin = end;
    	    *p = '\0';
    	    break;
    	case S_ENDEOLN:
    	    xargv[cnt++] = line + begin;
    	case S_EOLN:
    	    begin = end;
    	    *p = '\0';
    	    
    	    if (!fork())
    	    	exec(argv[1], xargv);
    	    	
    	    for (int i = (cnt = argc - 1); i < MAXARG; i++)
    	    	xargv[i] = 0;
    	    wait(0);
    	default:
    	    break;
	}
	
	p++;
    }
    exit(0);
}
