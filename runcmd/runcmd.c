/*  runcmd.c - source code of libruncmd
    Copyright (c) Danilo Tedeschi 2016  <danfyty@gmail.com>

    This file is part of Jucilei.

    jucilei is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    jucilei is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with jucilei.  If not, see <http://www.gnu.org/licenses/>.

 */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <runcmd.h>
#include "debug.h"

/*
if runcmd returns -1, it could be pipe error, fork error, or wait for child process error
 */
int runcmd (const char *command, int *result, int *io) {

    int i, pipers, pipefd[2], aux, status, tmp_result;
    char *cmd;
    pid_t pid;

    tmp_result=0;
    cmd = malloc(sizeof(char) * (strlen(command)+1));

    pipers = pipe(pipefd);

    sysfail (pipers==-1, -1);

    pid = fork();
    sysfail (pid<0, -1);

    if (pid > 0) { /*parent*/

        close (pipefd[1]); /*close writing end*/
        aux = wait(&status); 

        sysfail (aux<0, -1);
        if (WIFEXITED(status)) {

            tmp_result |= NORMTERM | WEXITSTATUS(status);

            if (!read(pipefd[0], NULL, 1)) { /*this means that the child didn't write something in it, which only happens if exec fails*/
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
        write (pipefd[1], "1", 1);
        close (pipefd[1]);
        /*free(cmd);*/
        exit(EXECFAILSTATUS);
    }
    if (result)
        *result=tmp_result;

    close(pipefd[0]);
    free(cmd);
    return pid;
}

void (*runcmd_onexit)(void) = NULL;
