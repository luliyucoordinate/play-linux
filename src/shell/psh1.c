#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#define MAXARGS 20
#define ARGLEN 100

int execute(char *arglist[])
{
    execvp(arglist[0], arglist);
    perror("execvp failed");
    exit(1);
}

char *makestring(char *buf)
{
    buf[strlen(buf) - 1] = '\0';
    char *cp = malloc(strlen(buf)+1);
    if (cp == NULL)
    {
        fprintf(stderr, "no memory\n");
        exit(1);
    }
    strcpy(cp, buf);
    return cp;
}

int main()
{
    char *arglist[MAXARGS];
    int numargs = 0;
    char argbuf[ARGLEN];

    while (numargs < MAXARGS) 
    {
        printf("Arg[%d]? ", numargs);
        if (fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n')
        {
            arglist[numargs++] = makestring(argbuf);
        }
        else
        {
            if (numargs > 0)
            {
                arglist[numargs] = NULL;
                execute(arglist);
                numargs = 0;
            }
        }
    }
    return 0;
}