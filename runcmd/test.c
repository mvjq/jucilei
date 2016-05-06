/*
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
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

    char cmd[1024];
    int res, io[2];

    fgets(cmd, 1023, stdin);
    cmd[strlen(cmd)-1]='\0';

    runcmd(cmd, &res, io);

    printf("[res]=%d\n", res);


    printf("IS_EXECOK %d\n", IS_EXECOK(res));
    printf("IS_NORMTERM %d\n", IS_NORMTERM(res));
    printf("EXITSTATUS %d\n", EXITSTATUS(res));


    return 0;
}
