#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "runcmd.h"
#include "debug.h"

int runcmd(const char *command, int *result, int *io) {
    int i, pid, status;


    pid=fork();

    if(pid>0) { /*parent*/
       wait(&status); 
    } 
    else { /*child*/
        char cmd=malloc(sizeof(char) * (strlen(command)+1));
        char *args[RCMD_MAXARGS], *token;
        int rs;

        strcpy(cmd, command);
        token = strtok(cmd, RCMD_DELIM); 
        for(i=0;i<RCMD_MAXARGS && token!=NULL;++i) {
            args[i]=token;
            token=strtok(NULL, RCMD_DELIM);
        }
        args[i]=NULL;
        rs=execvp(args[0], args);
    }
    return 0;
}
