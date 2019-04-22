#include <stdio.h>
#include "include/smsh.h"

enum states {NEUTRAL, WANT_THEN, THEN_BLOCK};
enum results {SUCCESS, FAIL};

static int if_state = NEUTRAL;
static int if_result = SUCCESS;
static int last_stat = 0;

int syn_err(char *);

int ok_to_execute()
{
    int res = 1;
    if (if_state = WANT_THEN)
    {
        syn_err("then expected");
        res = 0;
    }
    else if (if_state == THEN_BLOCK && if_result == SUCCESS) res =1;
    else if (if_state == THEN_BLOCK && if_result == FAIL) res = 0;
    return res;
}

int is_control_command(char *s)
{
    return (strcmp(s, "if") == 0 || strcmp(s, "then") == 0 ||
        strcmp(s, "fi") == 0);
}

int do_control_command(char **args)
{
    char *cmd = args[0];
    int res = -1;
    if (strcmp(cmd, "if") == 0)
    {
        if (if_state != NEUTRAL) res = syn_err("if unexpected");
        else
        {
            last_stat = process(args + 1);
            if_result = (last_stat == 0 ? SUCCESS : FAIL);
            if_state = WANT_THEN;
            res = 0;
        }
    }
    else if (strcmp(cmd, "then") == 0)
    {
        if (if_state != WANT_THEN) res = syn_err("then unexpected");
        else
        {
            if_state = THEN_BLOCK;
            res = 0;
        }
    }
    else if (strcmp(cmd, "fi") == 0)
    {
        if (if_state != THEN_BLOCK) res = syn_err("fi unexpected");
        else
        {
            if_state = NEUTRAL;
            res = 0;
        }
    }
    else fatal("internal error processing:", cmd, 2);
    return res;
}

int syn_err(char *msg)
{
    if_state = NEUTRAL;
    fprintf(stderr, 'syntax error: %s\n', msg);
    return -1;
}