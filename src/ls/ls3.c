#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>

void do_ls(char []);
void do_stat(char* filename);
void show_file_info(char *fname, struct stat* buf);
void mode_to_letters(int mode, char str[]);
char *uid_to_name(uid_t);
char *gid_to_name(gid_t);

void mode_to_letters(int mode, char str[])
{
    strcpy(str, "----------");
    if (S_ISDIR(mode)) str[0] = 'd';
    if (S_ISCHR(mode)) str[0] = 'c';
    if (S_ISBLK(mode)) str[0] = 'b';
    if (S_ISREG(mode)) str[0] = 'r';
    if (S_IFIFO(mode)) str[0] = 'f';
    if (S_IFLNK(mode)) str[0] = 'l';
    if (S_IFSOCK(mode)) str[0] = 's';

    if (mode & S_IRUSR) str[1] = 'r';
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';

    if (mode & S_IRGRP) str[4] = 'r';
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';

    if (mode & S_IROTH) str[7] = 'r';
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) str[9] = 'x';
}

void show_file_info(char *fname, struct stat* buf)
{
    char str[] = "----------";
    mode_to_letters(buf->st_mode, str);

    printf("%s", str);
    printf("%4d ", (int)buf->st_nlink);
    printf("%-4s ", uid_to_name(buf->st_uid));
    printf("%-4s ", gid_to_name(buf->st_gid));
    printf("%8ld ", (long)buf->st_size);
    printf("%.12s ", ctime(&buf->st_mtime));
    printf("%s\n", fname);
}

void do_ls(char dirname[])
{
    DIR* dir_ptr;
    struct dirent* direntp;
    if ((dir_ptr = opendir(dirname)) == NULL)
    {
        fprintf(stderr, "ls3: cannot open %s\n", dirname);
    }
    else
    {
        while ((direntp = readdir(dir_ptr)) != NULL)
	{
            do_stat(direntp->d_name);
	}
	closedir(dir_ptr);
    }
}

void do_stat(char* filename)
{
    struct stat info;
    if (stat(filename, &info) == -1) perror(filename);
    else show_file_info(filename, &info);
}

char *uid_to_name(uid_t uid)
{
    struct passwd* pw_ptr;
    static char numstr[10];

    if ((pw_ptr = getpwuid(uid)) == NULL)
    {
        sprintf(numstr, "%d", uid);
	return numstr;
    }else return pw_ptr->pw_name;
}

char *gid_to_name(gid_t gid)
{
    struct group* grp_ptr;
    static char numstr[10];

    if ((grp_ptr = getgrgid(gid)) == NULL)
    {
        sprintf(numstr, "%d", gid);
	return numstr;
    }
    else return grp_ptr->gr_name;
}

int main(int argc, char *argv[])
{
    if (argc == 1) do_ls(".");
    else 
    {
        while (--argc)
	{
	    printf("%s:\n", *(++argv));
	    do_ls(*argv);
	}
    }
}
