#include <stdio.h>
#include "include/smsh.h"

int is_control_command(char *);
int do_control_command(char **);
int ok_to_execute();

int process(char **args)
{
    int res;
    if (args[0] == NULL) res = 0;
    else if (is_control_command(args[0])) res = do_control_command(args);
    else if (ok_to_execute()) res = execute(args);
    return res;
}