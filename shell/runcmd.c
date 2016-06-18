/*  runcmd.c - source code of jucilei
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
#include <signal.h>
#include <fcntl.h>
#include "utils.h"
#include "runcmd.h"


pid_t runcmd (const char *command, int input_redir, int output_redir, int error_redir) {
    int aux;
    pid_t pid;
    pid = fork();

    /*returns -1 if fork failed*/
    sysfail (pid < 0, -1);

    if (pid == 0) { /*child*/
        size_t i;
        char *args[RUNCMD_MAXARGS], *token, *cmd;   
        cmd = malloc (sizeof (char) * (strlen (command)+1));
        strcpy (cmd, command);
        token = strtok (cmd, RUNCMD_DELIM);
        for (i=0; i<RUNCMD_MAXARGS && token != NULL; ++i) {
            args[i]=token;
            token = strtok (NULL, RUNCMD_DELIM);
        }
        args[i] = NULL;
        /*io redirection*/
        if (input_redir != STDIN_FILENO)
            if (dup2 (input_redir, STDIN_FILENO)<0)
                goto error_release;
        if (output_redir != STDOUT_FILENO)
            if (dup2 (input_redir, STDOUT_FILENO)<0)
                goto error_release;
        if (error_redir != STDERR_FILENO)
            if (dup2 (input_redir, STDERR_FILENO)<0)
                goto error_release;

        execvp (args[0], args);
                        
        error_release:
        free (cmd);
        return RUNCMD_FAILURE;
    }
    return pid;

}
