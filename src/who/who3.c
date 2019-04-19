#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <utmplib.h>

#define SHOWHOST

void show_info(struct utmp* utbufp);
void show_time(struct timeval* utimep);


void show_info(struct utmp* utbufp)
{
    printf("%-8.8s", utbufp->ut_name);
    printf(" ");
    printf("%-8.8s", utbufp->ut_line);
    printf(" ");
    show_time(&(utbufp->ut_tv));
    printf(" ");
#ifdef SHOWHOST
    printf("(%s)", utbufp->ut_host);
#endif
    printf("\n");
}

void show_time(struct timeval* utimep)
{
   struct tm* lt = localtime((time_t*)&(utimep->tv_sec));
   char str_time[100];
   strftime(str_time, sizeof str_time, "%Y-%m-%d %H:%M:%S", lt);
   printf("%s", str_time);
}


int main()
{
    struct utmp *utbufp, *utmp_next();

    if (utmp_open(UTMP_FILE) == -1)
    {
        perror(UTMP_FILE);
        exit(1);
    }

    while ((utbufp = utmp_next()) != NULL)
    {
        show_info(utbufp);
    }
    utmp_close();
    return 0;
}
