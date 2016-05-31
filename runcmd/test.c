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

int main(int argc, char **argv) {

    char cmd[1024];
    int res, io[3];
    char *fil[3]={"input.txt", "output.txt", "err.txt"};
    const char *c1 = "sleep 1 &";

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


    /*runcmd(cmd, &res, NULL); */
    runcmd(c1, &res, NULL);

    printf("[res]=%d\n", res);

    /*close(io[STDIN_FILENO]);
    close(io[STDOUT_FILENO]);
    close(io[STDERR_FILENO]);
    */

    printf("IS_EXECOK %d\n", IS_EXECOK(res));
    printf("IS_NORMTERM %d\n", IS_NORMTERM(res));
    printf("EXITSTATUS %d\n", EXITSTATUS(res));


    return 0;
}
