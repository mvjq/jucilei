#ifndef PARSER_H
#define PARSER_H
#include <search.h>

#define IS_NONBLOCK (x) ((x)==NONBLOCK_CHAR)
#define NONBLOCK_CHAR '&'
#define PIPE_STR "|"
#define INPUT_CHAR '<'
#define OUTPUT_CHAR '<'
#define IS_IO_REDIR (x) (((x)==INPUT_CHAR)||((x)==OUTPUT_CHAR))

struct qelem {
    struct qelem *q_forw;
    struct qelem *q_back;
    char *q_data;
};

typedef struct cmd_line_t {
    int io[3]; /*standard is STDIN_FILENO...*/
    int is_nonblock;
    struct qelem *pipe_list_tail; /*note that if there's no pipe this is a one element list, check <search.h> to see struct qelem */
    size_t pipe_list_size; /*number of commands executed with pipes*/
} cmd_line_t;


/*allocate memory for parser*/
cmd_line_t* new_cmd_line (void); 

/*
   parses the cmd string, return -1 in case of error
 */
int parse_cmd_line (const char *cmd, cmd_line_t *cmd_line) ; 

/*releases all the allocated memory*/
void release_cmd_line (cmd_line_t *cmd_line);

#endif
