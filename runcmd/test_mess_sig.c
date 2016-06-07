/*  test.c - receives a command line and uses libruncmd to execut it and shows the return values

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
#include <runcmd.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

void f1() {
    puts("A");
}
void f2() {
    puts("B");
}
void f3() {
    puts("C");
}
void f4() {
    puts("D");
}

int main(int argc, char **argv) {

    char cmd[1024];
    int i, res, io[3];

    char *fil[3]={"input.txt", "output.txt", "err.txt"};
    void (*ffx[4]) (void);
    struct sigaction act;

    const char *cmd1[] = 
    {"sleep 5 &",
    "sleep 4 &",
    "sleep 3 &",
    "sleep 2 &",
    "sleep 1 &" };

    ffx[0]=f1;
    ffx[1]=f2;
    ffx[2]=f3;
    ffx[3]=f4;

    /*io[STDIN_FILENO]=open(fil[STDIN_FILENO], O_RDONLY);
    io[STDOUT_FILENO]=open(fil[STDOUT_FILENO], O_WRONLY);
    io[STDERR_FILENO]=open(fil[STDERR_FILENO], O_WRONLY);
    */

    if (argc > 1)
        strcpy(cmd, argv[1]);
    else {
        /*fgets(cmd, 1023, stdin); */
        /*cmd[strlen(cmd)-1]='\0'; */
    }

    sigaction (SIGCHLD, NULL, &act);

    if (act.sa_handler == SIG_DFL)
        puts("OK Before!");

    /*runcmd(cmd, &res, NULL); */
    for (i=0;i<2;i++) {
        runcmd_onexit=ffx[i];
        runcmd(cmd1[i], &res, NULL);
        printf("%s -> [res]=%d\n", cmd1[i], res);

        /*close(io[STDIN_FILENO]);
          close(io[STDOUT_FILENO]);
          close(io[STDERR_FILENO]);
         */
        printf("IS_EXECOK %d\n", IS_EXECOK(res));
        printf("IS_NORMTERM %d\n", IS_NORMTERM(res));
        printf("EXITSTATUS %d\n", EXITSTATUS(res));
        printf("IS_NONBLOCK %d\n", IS_NONBLOCK(res));
        sleep (1);
    }
    sleep (2);


    sigaction (SIGCHLD, NULL, &act);

    if (act.sa_handler != SIG_DFL)
        puts("ERROR! Mess up the default signal!");
    


    return 0;
}
