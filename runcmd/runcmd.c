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
#include <signal.h>
#include <runcmd.h>
#include <fcntl.h>
#include "debug.h"

/*
TODO: use insque and remque in <search.h> 
 */

/*list of callback functions for nonblocking executions of runcmd*/
typedef struct nonblocking_callback_t {
    pid_t pid; /*child process pid*/
    struct nonblocking_callback_t *next; /*pointer to the next if this is set to NULL this is the last one*/
    void (*runcmd_onexit) (void);
} nonblocking_callback_t;

nonblocking_callback_t *nonblocking_callback_list = NULL;

void (*old_handler) (int) = NULL; 

/**/
void _sigchld_handler (int sig, siginfo_t *p_info, void *vptr) {

    nonblocking_callback_t *ptr = nonblocking_callback_list, *ptr_prv=NULL, *ptr_aux=NULL;
    int status=0;
    /*we have to go thru all the children and check if it's dead*/
    /*Not working yet!*/

    while (ptr != NULL) {
        /* WNOHANG returns immediately if nothing happend to the process
           waitpid returns 0 if nothing happend
           */
        if (waitpid (ptr->pid, &status, WNOHANG)) {
            ptr->runcmd_onexit();
            if (ptr_prv)
                ptr_prv->next=ptr->next;
            else nonblocking_callback_list=ptr->next;
            ptr_aux = ptr;
            ptr=ptr_prv;
            free (ptr_aux);
        }
        ptr_prv=ptr;
        ptr = (ptr) ? ptr->next : NULL;
    } 

    if (!ptr && old_handler != NULL) {
        struct sigaction act;
        memset (&act, 0, sizeof act);
        act.sa_handler = old_handler; 
        sigaction (SIGCHLD, &act, NULL);
    }

}

void register_child_callback (pid_t pid) {
    nonblocking_callback_t *new_head = malloc (sizeof (nonblocking_callback_t));
    new_head->runcmd_onexit = runcmd_onexit;
    new_head->next = nonblocking_callback_list;
    new_head->pid = pid;
    nonblocking_callback_list = new_head;
}


/*
if runcmd returns -1, it could be pipe error, fork error, or wait for child process error
if *io is not NULL it should have 3 entries, otherwise we will have segmentation fault
 */
int runcmd (const char *command, int *result, const int *io) {

    int i, pipers, pipefd[2], aux, status, tmp_result;
    char *cmd;
    pid_t pid;


    tmp_result=0;

    pipers = pipe(pipefd);
    sysfail (pipers==-1, -1);

    pid = fork();
    sysfail (pid<0, -1);

    if (pid > 0) { /*parent*/
        char *pch; /*this is only used to get the return from memchr*/

        close (pipefd[1]); /*close writing end*/

        /*checks if '&' is present in the string command*/
        pch = memchr(command, RCMD_NONBLOCK, strlen(command));
        tmp_result |= pch!=NULL ? NONBLOCK : 0;

        if (IS_NONBLOCK(tmp_result)) {

            struct sigaction act, oact;
            sigaction (SIGCHLD, NULL, &oact);

            old_handler = oact.sa_handler;

            memset (&act, 0, sizeof act);
            act.sa_flags = SA_SIGINFO;
            act.sa_sigaction = _sigchld_handler; 

            /*register a callback for the child with the current runcmd_onexit as callback function*/
            register_child_callback (pid);

            /*assign void _sig_handler(int) to SIGCHLD*/
            sysfail (sigaction(SIGCHLD, &act, NULL)<0, -1);
        }
        else {
            aux = wait(&status); 

            sysfail (aux<0, -1);
            if (WIFEXITED(status)) {

                tmp_result |= NORMTERM | WEXITSTATUS(status);

                /*this means that the child didn't write something in it, which only happens if exec fails*/
                tmp_result |= !read(pipefd[0], &aux, 1) ? EXECOK: 0; 
            }
        }
        close (pipefd[0]);
    } 
    else { /*child*/
        char *args[RCMD_MAXARGS], *token;
        cmd = malloc(sizeof(char) * (strlen(command)+1));
        strcpy(cmd, command);

        close (pipefd[0]); /*close the unused pipe end*/

        aux = fcntl (pipefd[1], F_SETFD, FD_CLOEXEC); /*set close on exec flag*/
        sysfail (aux==-1, -1);

        /*parse the comand given*/
        token = strtok(cmd, RCMD_DELIM);
        for (i = 0; i < RCMD_MAXARGS && token != NULL && token[0]!='&'; ++i) {
            args[i] = token;
            token = strtok(NULL, RCMD_DELIM);
        }
        args[i] = NULL;

        /*now we have to make the file descriptors in *io to be the new stdin,stdout,stderr*/
        if (io) {
            int std_fd[]={STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO};
            for (i = 0;i < 3;i++) {
                if (io[std_fd[i]] > 0) {
                    aux = dup2(io[std_fd[i]], std_fd[i]);
                    sysfail (aux<0, -1);
                }
            }
        }
        /*end of redirection of IO*/

        execvp(args[0], args);

        /*if we got here, it means args[0] can't be executed :(*/
        write (pipefd[1], "1", 1);
        close (pipefd[1]);
        free (cmd);
        exit (EXECFAILSTATUS);
    }

    if (result)
        *result=tmp_result;
    return pid;
}

void (*runcmd_onexit)(void) = NULL;
