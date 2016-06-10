#include <search.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"
#include "debug.h"

#define IS_NONBLOCK(x) ((x)==NONBLOCK_CHAR)
#define IS_INPUT_REDIR(x) ((x)==INPUT_REDIR_CHAR)
#define IS_OUTPUT_REDIR(x) ((x)==OUTPUT_REDIR_CHAR)
#define IS_IO_REDIR(x) (((x)==INPUT_REDIR_CHAR)||((x)==OUTPUT_REDIR_CHAR))
#define IS_PIPE(x) ((x)==PIPE_CHAR)

cmd_line_t* new_cmd_line (void) {
    cmd_line_t *cmd = malloc (sizeof (cmd_line_t));
    sysfail (cmd==NULL, NULL);
    cmd->io[0] = cmd->io[1] = cmd->io[2] = NULL;
    cmd->pipe_list_tail = NULL;
    cmd->pipe_list_size = 0;
    cmd->is_nonblock = 0;
    return cmd;
}

/*
   copies the first n bytes of a string
    also puts a \0 at the end 
    also trims the string
 */
char *stringndup(const char *str, size_t n) {
    char *rstr = malloc (sizeof (char) * n);
    int poff=0;

    /*triming*/
    while (cmd_str[poff] != '\0' && (iscntrl (cmd_str[poff]) || isblank (cmd_str[poff])))
        poff++;
    n -= poff;
    while (n>0 && iscntrl (cmd_str[poff + n-1]) || isblank (cmd_str[poff + n - 1]))
        --n;
    /*end triming*/

    sysfail (rstr==NULL, NULL);
    rstr[n] = '\0';
    strncpy (rstr, str, n);
    return rstr;
}

/*cmd_str -> start of the command
    n     -> number of bytes which is in the command 
 */
struct qelem *new_qelem (const char *cmd_str, size_t n) {
    struct qelem *rqelem = malloc (sizeof (struct qelem));
    sysfail (rqelem==NULL, NULL);
    rqelem->q_forw = NULL;
    rqelem->q_back = NULL;
    rqelem->q_data = (char *)stringndup (cmd_str, n);
    return rqelem;
}

/*
returns -1 in case of error 
 */
int pipe_list_add_cmd (cmd_line_t *cmd_line, const char *cmd_str, size_t n) {
    int i;
    struct qelem *nelem = new_qelem (cmd_str, n); 
    sysfail (nelem==NULL, -1);
    insque (nelem, cmd_line->pipe_list_tail);
    cmd_line->pipe_list_tail = nelem;
    return EXIT_SUCCESS;
}


/*
returns:
0 in case of succes 
1 in ..
2 in syntax error
-1 in unexpected error
 */
int parse_cmd_line (cmd_line_t *cmd_line, const char *cmd) {
    size_t prv_cmd_beg = 0, i, cmd_len;

    /*used to get return of functions*/
    int raux;
    /*STDIN, STDOUT, STDERR*/
    int io_redir_seen=-1;
    /*return status*/
    int ret = 0;

    sysfail (cmd_line==NULL, -1);
    cmd_len = strlen (cmd);

    /*assuming that the last thing is io redirection or nonblocking indicator*/
    for (i=0; i < cmd_len && !IS_IO_REDIR (cmd[i]) && !IS_NONBLOCK (cmd[i]); ++i) {
        if (IS_PIPE (cmd[i])) {
            raux = pipe_list_add_cmd (cmd_line, cmd + prv_cmd_beg, i-prv_cmd_beg);
            sysfail (raux<0, -1);
            prv_cmd_beg = i + 1;
        }
    }
    if (prv_cmd_beg < cmd_len) {
        raux = pipe_list_add_cmd (cmd_line, cmd + prv_cmd_beg, i-prv_cmd_beg);
        sysfail (raux<0, -1);
        prv_cmd_beg = i + 1;
    }
    for (;i < cmd_len && !IS_NONBLOCK(cmd[i]); ++i) {
        /*todo: recognize output redir*/
        if (IS_INPUT_REDIR (cmd[i])) {
            if (io_redir_seen != -1)
                cmd_line->io[io_redir_seen] = stringndup (cmd + prv_cmd_beg, i-prv_cmd_beg);
            prv_cmd_beg = i + 1;
            io_redir_seen = 0;
        }
        else if (IS_OUTPUT_REDIR (cmd[i])) {
            if (io_redir_seen != -1)
                cmd_line->io[io_redir_seen] = stringndup (cmd + prv_cmd_beg, i-prv_cmd_beg);
            io_redir_seen = 1;
        }
        else if (IS_PIPE (cmd[i]))
                ret |= 2;
    }
    if (prv_cmd_beg < cmd_len && io_redir_seen != -1) 
        cmd_line->io[io_redir_seen] = stringndup (cmd + prv_cmd_beg, i-prv_cmd_beg);

    /*if it is non_block it must be the last thing*/
    cmd_line->is_nonblock = i < cmd_len && IS_NONBLOCK (cmd[i]);
    
    return ret;
} 

void print (cmd_line_t *cmd_line) {
    int c = 0;
    struct qelem *ptr = cmd_line->pipe_list_tail;
    printf("IS_NONBLOCK %d\n", cmd_line->is_nonblock);

    if (cmd_line->io[0] != NULL)
        printf("INPUT %s\n", cmd_line->io[0]);
    if (cmd_line->io[1] != NULL)
        printf("OUTPUT %s\n", cmd_line->io[1]);

    printf("IS_NONBLOCK %d\n", cmd_line->is_nonblock);
    while (ptr != NULL) {
        printf("%d: %s *\n", ++c, ptr->q_data);

        ptr = ptr->q_back;
    }
    puts("------\n");
}

void release_cmd_line (cmd_line_t *cmd_line) {
    if (cmd_line != NULL)
        free (cmd_line);
}


/*
test parser 
 */
int main(void) {
    int raux;
    cmd_line_t *cmd_line = new_cmd_line();

    char cmd[555];
    while (fgets (cmd, 554, stdin) != NULL ) {
        raux = parse_cmd_line (cmd_line, cmd);
        if (raux & 2)
            puts ("Syntax Error!");
        else
            print (cmd_line);

    }




    return 0;
}
