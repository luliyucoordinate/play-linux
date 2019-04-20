#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define QUESTION "Do you want another transaction"
#define TRIES 3
#define SLEEPTIME 2
#define BEEP putchar('\a')

int get_response(char *, int);
int get_ok_char();
void set_cr_noecho_mode();
void set_nodelay_mode();
void tty_mode(int how);

int get_response(char *question, int maxtries)
{
    printf("%s (y/n)?", question);
    fflush(stdout);
    while (1)
    {
        int input;
        sleep(SLEEPTIME);
        input = tolower(get_ok_char());
        if (input == 'y') return 0;
        if (input == 'n') return 1;
        if (maxtries-- == 0) return 2;
        BEEP;
    }
}

int get_ok_char()
{
    int c;
    while ((c = getchar()) != EOF && strchr("yYnN", c) == NULL);
    return c;
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

void set_nodelay_mode()
{
    int termflags = fcntl(0, F_GETFL);
    termflags |= O_NDELAY;
    fcntl(0, F_SETFL, termflags);
}
void tty_mode(int how)
{
    static struct termios original_mode;
    static int original_flags;
    if (how == 0) 
    {
        tcgetattr(0, &original_mode);
        original_flags = fcntl(0, F_GETFL);
    }
    else 
    {
        tcsetattr(0, TCSANOW, &original_mode);
        fcntl(0, F_SETFL, original_flags);
    }
}

int main()
{
    tty_mode(0);
    set_cr_noecho_mode();
    set_nodelay_mode();
    int response = get_response(QUESTION, TRIES);
    tty_mode(1);
    return response;
}