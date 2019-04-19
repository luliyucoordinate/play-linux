#ifndef UTMPLIB_H_
#define UTMPLIB_H_

int utmp_open(char * filename);
int utmp_reload();
struct utmp* utmp_next();
void utmp_close();

#endif
