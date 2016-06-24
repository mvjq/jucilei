/*  shell.c - source code of jucilei
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "utils.h"
#include "parser.h"
#include "process.h"
#include "job.h"

/**/
qelem *job_list_head, *job_list_tail;

/*
   job which is currently running as foreground
 */
job_t *fg_job = NULL;

#define IS_FG_JOB(job) (((job_t*)(job))==fg_job) 

/*
TODO: put this in utils.h as a define function 
 */
void job_list_rem (qelem *q) {
    remque (q);
    if (q == job_list_head)
        job_list_head = job_list_head->q_forw;
    if (q == job_list_tail)
        job_list_tail = job_list_tail->q_back;
    release_job ((job_t*) q->q_data);
    free (q);
}

int shell_init () {
    fg_job = NULL;
    job_list_head = job_list_tail = NULL;
    signal (SIGINT, SIG_IGN);
    signal (SIGQUIT, SIG_IGN);
    signal (SIGTSTP, SIG_IGN);
    signal (SIGTTIN, SIG_IGN);
    signal (SIGTTOU, SIG_IGN);
    signal (SIGCHLD, SIG_IGN);
    return EXIT_SUCCESS;
}

void _sigchld_handler (int signum) {
    qelem *p, *q;
    process_t *proc;
    job_t *job;

    for (q = job_list_head; q != NULL; q = q->q_forw) {
        
        char completed_all = 1;
        job = (job_t*) q->q_data;
        for (p = fg_job->process_list_head; p != NULL; p = p->q_forw) {

            proc = (process_t*) p->q_data;
            if (proc->completed)
                continue;
            if (waitpid (proc->pid, &proc->status, WNOHANG)) {
                proc->status = (WIFEXITED (proc->status)) ? WEXITSTATUS (proc->status): -1;
                proc->completed = 1;
            }
            else 
                completed_all = 0;
        }
        /*
        we do not remove now 
         */
        if (completed_all) {
            if (IS_FG_JOB (job)) 
                fg_job = NULL;
            job->completed = 1;
        }
    }
}

int run_fg_job() {
    while (fg_job != NULL)
        pause();
    return EXIT_SUCCESS;
}

/*
pushes job to the job_list 
 */
void job_list_push (job_t *job) {

    qelem *new_elem = malloc (sizeof (qelem));
    new_elem->q_data = (void *) job;
    insque (new_elem, job_list_tail);
    if (job_list_tail == NULL)
        job_list_head = new_elem;
    job_list_tail = new_elem;
}

/*
returns -1 in case of error (cmd coundn't be executed) 
 */
int create_job (const char *cmd) {

    int aux; /*used to get return values*/
    int ret = EXIT_SUCCESS; /*return value*/
    process_t *proc = NULL;
    cmd_line_t *cmd_line = NULL;
    qelem *ptr;
    job_t *job;
    struct sigaction oact, act;

    cmd_line = new_cmd_line();
    aux = parse_cmd_line (cmd_line, cmd);
    job = new_job(STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO);

    if (!IS_CMD_LINE_OK (aux)) {
        ret = aux;
        goto release_stuff;
    }

    for (ptr = cmd_line->pipe_list_head; ptr != NULL; ptr=ptr->q_forw) {
        proc = new_process (ptr->q_data);
        aux = job_push_process (job, proc);
        if (aux==-1) {
            ret = -1;
            goto release_stuff;
        }
    }

    for (ptr = job->process_list_head;ptr != NULL; ptr=ptr->q_forw) {
        printf ("%s\n", ((process_t*) ptr->q_data)->argv[0]);
    }

    /*running */
    aux = run_job (job);

    /*problem with running the job*/
    if (aux == -1) {
        ret = -1;
        goto release_stuff;
    }
    /*now we can consider the job is successfully begin executed*/
    
    if (!cmd_line->is_nonblock)
        fg_job = job;

    job_list_push (job);

    act.sa_handler = _sigchld_handler;
    sigemptyset (&act.sa_mask);
    act.sa_flags = 0;

    sigaction (SIGCHLD, NULL, &oact);

    if (oact.sa_handler != _sigchld_handler)
        sigaction (SIGCHLD, &act, NULL);
        

    /*print_job (job);*/


    /*printf ("%d\n", run_process(proc, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO)); */
    /*release_process (proc); */

    /*TODO: find a better name for this*/
release_stuff:
    release_cmd_line (cmd_line);
    return ret;
}
