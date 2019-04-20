#include <stdio.h>
#include <unistd.h>
#include <curses.h>

int main()
{
    initscr();
    clear();
    for (int i = 0; i < LINES; ++i)
    {
        move(i, i + i);
        if ((i & 1) == 1) standout();
        addstr("Hello, world");
        if ((i & 1) == 1) standend();
        sleep(1);
        refresh();
    }
    endwin();
}