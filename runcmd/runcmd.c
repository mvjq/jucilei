#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "runcmd.h"
#include "debug.h"

/*
if runcmd returns -1, it could be pipe error, fork error, or wait for child process error
TODO: show those error messages maybe
 */

int runcmd (const char *command, int *result, int *io) {

    int i, pipers, pipefd[2], aux, status, tmp_result;
    char *cmd;
    pid_t pid;

    tmp_result=0;
    cmd = malloc(sizeof(char) * (strlen(command)+1));

    pipers = pipe(pipefd);

    if (pipers == -1)
        return -1;

    pid = fork();
    if (pid < 0)
        return -1;

    if (pid > 0) { /*parent*/

        close (pipefd[1]); /*close writing end*/
        aux = wait(&status); 
        if (aux < 0)
            return -1;
        if (WIFEXITED(status)) {

            tmp_result |= NORTERM | WEXITSTATUS(status);

            if (!read(pipefd[0], &aux, 4)) { /*this means that the child didn't wrote something in it, which only happens if exec fails*/
                tmp_result |= EXECOK;
            }
        }
    } 
    else { /*child*/

        char *args[RCMD_MAXARGS], *token;
        close(pipefd[0]);
        strcpy(cmd, command);
        token = strtok(cmd, RCMD_DELIM); 
        for (i = 0; i < RCMD_MAXARGS && token != NULL; ++i) {
            args[i] = token;
            token = strtok(NULL, RCMD_DELIM);
        }

        args[i] = NULL;
        execvp(args[0], args);

        /*if we got here, it means args[0] can't be executed :(*/
        /*we got to do something to differ this case from the case where the process itself returns EXITFAILSTATUS*/
        /*I'm using pipe to send a message to the caller*/
        aux=EXITFAILSTATUS;
        write(pipefd[1], &aux, 4);
        close(pipefd[1]);
        exit(EXITFAILSTATUS);
    }
    if (result)
        *result=tmp_result;

    close(pipefd[0]);
    free(cmd);
    return pid;
}

void (*runcmd_onexit)(void) = NULL;
