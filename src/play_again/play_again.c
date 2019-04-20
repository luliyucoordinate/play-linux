#include <stdio.h>
#include <termios.h>

#define QUESTION "Do you want another transaction"

int get_response(char *);

int get_response(char *question)
{
    printf("%s (y/n)?", question);
    while (1)
    {
        switch (getchar())
        {
            case 'y':
            case 'Y': return 0;
            case 'n':
            case 'N':
            case EOF: return 1;
            default:
                break;
        }
    }
}

int main()
{
    int response = get_response(QUESTION);
    return response;
}