#include <stdio.h>
#include <termios.h>

#define QUESTION "Do you want another transaction"

int get_response(char *);
void set_cr_noecho_mode();
void tty_mode(int how);

int get_response(char *question)
{
    printf("%s (y/n)?", question);
    while (1)
    {
        int input;
        switch (input = getchar())
        {
            case 'y':
            case 'Y': return 0;
            case 'n':
            case 'N':
            case EOF: return 1;
        }
    }
}

void set_cr_noecho_mode()
{
    struct termios ttystate;
    tcgetattr(0, &ttystate);
    ttystate.c_lflag &= ~ICANON;
    ttystate.c_lflag &= ~ECHO;
    ttystate.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &ttystate);
}

void tty_mode(int how)
{
    static struct termios original_mode;
    if (how == 0) tcgetattr(0, &original_mode);
    else tcsetattr(0, TCSANOW, &original_mode);
}

int main()
{
    tty_mode(0);
    set_cr_noecho_mode();
    int response = get_response(QUESTION);
    tty_mode(1);
    return response;
}