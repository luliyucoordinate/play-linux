#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <fcntl.h>

#define MESSAGE "hello"
#define BLANK "     "

int row = 10, col = 0, dir = 1;
int delay = 200, done = 0;

void on_input(int signum)
{
    int c = getch();
    if (c == 'Q' || c == EOF) done = 1;
    else if (c == ' ') dir = -dir;
}

void on_alarm(int signum)
{
    signal(SIGALRM, on_alarm);
    mvaddstr(row, col, MESSAGE);
    refresh();

    if (dir == -1 && col <= 0) dir = 1;
    else if (dir == 1 && col + strlen(MESSAGE) >= COLS) dir = -1; 
}

void enable_kbd_signals()
{
    int fd_flags;
    fcntl(0, __F_SETOWN, getpid());
    fd_flags = fcntl(0, F_GETFL);
    fcntl(0, F_SETFL, (fd_flags | O_ASYNC));
}

int main()
{
    initscr();
    crmode();
    noecho();
    clear();

    signal(SIGIO, on_input);
    enbale_kbd_signals();
    signal(SIGALRM, on_alarm);
    set_ticker(delay);
    mvaddstr(row, col, MESSAGE);
    while (!done) pause();
    endwin();
}