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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"
#include "process.h"


int main (int argc, char *argv[]) {

    char cmd[256];
    process_t *proc = NULL;

    while (fgets (cmd, 256, stdin) != NULL) {
        if (cmd[strlen (cmd)-1] == '\n')
            cmd[strlen (cmd)-1] = '\0'; 
        proc = new_process (cmd);
        printf ("%d\n", run_process(proc, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO));
        release_process (proc);
    }



    return EXIT_SUCCESS;
}
