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

#ifndef RUNCMD_H
#define RUNCMD_H

#define RUNCMD_DELIM " \n\t\r"
#define RUNCMD_MAXARGS 256

#define RUNCMD_FAILURE 0
/*
returns the pid of the child
(input,output,error)_redir are file descriptors
 */
pid_t runcmd (const char *cmd, int input_redir, int output_redir, int error_redir);
#endif
