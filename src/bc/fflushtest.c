#include <stdio.h>
#include <string.h>
#include <unistd.h>
 
int main()
{
 
   char buff[1024];
 
   memset( buff, '\0', sizeof( buff ));
 
   fprintf(stdout, "启用全缓冲\n");
   setvbuf(stdout, buff, _IOFBF, 1024);
 
   fprintf(stdout, "这里是 runoob.com\n");
   fprintf(stdout, "该输出将保存到 buff\n");
   fflush( stdout );//立即写入
 
   fprintf(stdout, "这将在编程时出现\n");
   fprintf(stdout, "最后休眠五秒钟\n");
 
   sleep(5);
 
   return(0);
}