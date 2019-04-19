#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

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
    struct utmp current_record;
    int utmpfd;
    int reclen = sizeof current_record;
    if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1)
    {
	perror(UTMP_FILE);
	exit(1);
    }

    while (read(utmpfd, &current_record, reclen) == reclen)
    {
        show_info(&current_record);
    }
    close(utmpfd);
    return 0;
}
