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
#ifndef RUNCMD_H
#define RUNCMD_H

#define RCMD_MAXARGS 1024
#define RCMD_DELIM " \n\t\r" 
#define RCMD_MAXARGS 1024
#define RCMD_NONBLOCK '&'

#define NORMTERM (1 << 8)
#define EXECOK (1 << 9)
#define NONBLOCK (1 << 10)
#define RETSTATUS (0xFF)
#define EXECFAILSTATUS (127)

#define IS_NORMTERM(res) ((res & NORMTERM) && 1)
#define IS_NONBLOCK(res) ((res & NONBLOCK) && 1)
#define EXITSTATUS(res) ((res) & RETSTATUS)
#define IS_EXECOK(res) ((res & EXECOK) && 1)

int runcmd(const char *command, int *result, int *io);

extern void (*runcmd_onexit)(void);

#endif
