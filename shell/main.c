/*  main.c - source code of jucilei
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
#include "parser.h"
#include "process.h"
#include "job.h"


int main (int argc, char *argv[]) {

    char cmd[256];
    int ret;
    process_t *proc = NULL;
    cmd_line_t *cmd_line = NULL;
    qelem *ptr;
    job_t *job;

    while (fgets (cmd, 256, stdin) != NULL) {
        cmd_line = new_cmd_line();
        ret = parse_cmd_line (cmd_line, cmd);
        job = new_job(STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO);

        if (IS_SYNTAX_ERROR (ret)) {
            puts ("Syntax Error");
            goto release_stuff;
        }

        for (ptr = cmd_line->pipe_list_head; ptr != NULL; ptr=ptr->q_forw) {
            proc = new_process (ptr->q_data);
            ret = job_add_process (job, proc);
        }

        /*running */
        for (ptr = job->process_list_head;ptr != NULL; ptr=ptr->q_forw) {
            printf ("%s\n", ((process_t*) ptr->q_data)->argv[0]);
        }
        /*printf ("%d\n", run_process(proc, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO)); */
        /*release_process (proc); */

        /*TODO: find a better name for this*/
        release_stuff:
        release_job (job);
        release_cmd_line (cmd_line);
    }



    return EXIT_SUCCESS;
}
