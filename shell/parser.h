/*  parser.h - source code of jucilei
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
#ifndef PARSER_H
#define PARSER_H
#include "utils.h"

#define NONBLOCK_CHAR '&'
#define PIPE_STR "|"
#define PIPE_CHAR '|'
#define INPUT_REDIR_CHAR '<'
#define OUTPUT_REDIR_CHAR '>'

#define SYNTAX_ERROR (1<<1)
#define IS_SYNTAX_ERROR(x) (((x) & SYNTAX_ERROR) && 1)


typedef struct cmd_line_t {
    char *io[3]; /*standard is {NULL,NULL,NULL}..., meaning no redirection*/
    int is_nonblock;
    struct qelem *pipe_list_tail, *pipe_list_head; /*note that if there's no pipe this is a one element list, check <search.h> to see struct qelem */
} cmd_line_t;


/*allocate memory for parser*/
cmd_line_t* new_cmd_line (void); 

/*
   parses the cmd string, return -1 in case of error
notes: & must be the last thing (anything after will be ignored)
        (<,>) must come after all the pipes
 */
int parse_cmd_line (cmd_line_t *cmd_line, const char *cmd) ; 

/*releases all the allocated memory*/
void release_cmd_line (cmd_line_t *cmd_line);

#endif
