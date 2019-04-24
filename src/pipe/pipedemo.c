#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <stdlib.h>


int main()
{
    int len, apipe[2];
    char buf[BUFSIZ];
    if (pipe(apipe) == -1)
    {
        perror("could not make pipe");
        exit(1);
    }

    printf("Got a pipe!");
    while (fgets(buf, BUFSIZ, stdin))
    {
        len = strlen(buf);
        if (write(apipe[1], buf, len) != len)
        {
            perror("writing to pipe");
            break;
        }

        memset(buf, 'X', len);
        len = read(apipe[0], buf, BUFSIZ);
        if (len == -1)
        {
            perror("reading from pipe");
            break;
        }

        if (write(1, buf, len) != len)
        {
            perror("writing to stdout");
            break;
        }
    }
}