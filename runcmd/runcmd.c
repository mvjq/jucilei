#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "runcmd.h"
#include "debug.h"

int runcmd(const char *command, int *result, int *io) {
    int i, aux, status, tmp_result;
    pid_t pid;
    tmp_result=0;

    pid=fork();
    if(pid < 0)
        return -1;

    /*printf("[pid]=%d [cmd]=%s\n", pid, command);*/


    if(pid > 0) { /*parent*/
       aux = wait(&status); 
       if(aux < 0)
           return -1;
       if( WIFEXITED(status)) {
           tmp_result |= NORTERM | WEXITSTATUS(status);
           if(WEXITSTATUS(status) != EXITFAILSTATUS)
               tmp_result |= EXECOK;
       }
    } 
    else { /*child*/
        char *cmd = malloc(sizeof(char) * (strlen(command)+1));
        char *args[RCMD_MAXARGS], *token;
        
        strcpy(cmd, command);
        token = strtok(cmd, RCMD_DELIM); 
        for(i = 0; i < RCMD_MAXARGS && token != NULL; ++i) {
            args[i] = token;
            token = strtok(NULL, RCMD_DELIM);
        }
        args[i] = NULL;
        execvp(args[0], args);
        /*if we got here, it means args[0] can't be executed :(*/
        /*we got to do something to differ this case from the case where the process itself returns EXITFAILSTATUS*/
        raise(SIGILL);
        exit(EXITFAILSTATUS);
    }
    if(result)
        *result=tmp_result;
    return pid;
}

void (*runcmd_onexit)(void) = NULL;
