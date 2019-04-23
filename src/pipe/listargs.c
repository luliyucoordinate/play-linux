#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("Numver of args:%d, Args are: \n", argc);
    for (int i = 0; i < argc; ++i)
    {
        printf("args[%d] %s\n", i, argv[i]);
    }
    fprintf(stderr, "This message is sent to stderr.\n");
}