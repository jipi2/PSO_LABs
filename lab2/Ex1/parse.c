#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tiny.h"

extern char command[BUFFSIZE][BUFFSIZE];
extern int command_size;
extern char redirectFile[BUFFSIZE];
extern char command2[256][256];
extern int command2_size;

char* replaceEnvValues(char *token)
{
    char *value = strdup(token);
    value = strtok(value,"$");
    value = getenv(value);
   
    if(value==NULL)
        return token;
    else
    {
        return value;
    }
}

void setEnvValue(char *token)
{
    char *copy = strdup(token);
    char *name = strtok(copy," =");
    char *value = strtok(NULL," =");
    setenv(name, value,1);
}

int parse(const char *buffer)
{
    char *token = NULL;
    int ret;
    char *buffer_copy = strdup(buffer);
    int redir = 0;
    int token_pipe = 0;

    ret = strcmp(buffer_copy,"\n");
    if(ret == 0) 
        return NEW_LINE;

    token = strtok(buffer_copy, " \t\n");
    ret = strcmp(buffer_copy, STRING_EXIT);
    if(ret == 0)
        return COMM_EXIT;

    while(token != NULL)
    {
        if(token[0] == '$')
            strncpy(command[command_size],replaceEnvValues(token),BUFFSIZE);

        else if(strstr(token,"="))
            {
                setEnvValue(token);
                return COMM_SIMPLE;
            }
        else if (strcmp(token,">")==0)
        {
            token = strtok(NULL," \t\n");
            strncpy(redirectFile,token,BUFFSIZE);
            redir = 1;
        }
        else
        {
            if (token[0] == '|' && strlen(token) == 1)
            {
                token_pipe = 1;
                token = strtok(NULL," \t\n");   
            }
            if(token_pipe == 1)
            {
                strncpy(command2[command2_size],token,BUFFSIZE);
                command2_size++;
            }
            else
            { 
                strncpy(command[command_size],token,BUFFSIZE);
                command_size++;
            }
        }

        token = strtok(NULL," \t\n");
    }

    free(buffer_copy);

    if(redir == 1)
        return COMM_REDIRECT;
    if(token_pipe == 1)
        return COMM_PIPE;

    return COMM_SIMPLE;
}