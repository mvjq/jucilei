#include <search.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"
#include "debug.h"

cmd_line_t* new_cmd_line (void) {
    cmd_line_t *cmd = malloc (sizeof (cmd_line_t));
    sysfail (cmd==NULL, NULL);
    cmd->io[0] = STDIN_FILENO;
    cmd->io[1] = STDOUT_FILENO;
    cmd->io[2] = STDERR_FILENO;
    cmd->pipe_list_tail = NULL;
    cmd->pipe_list_size = 0;
    cmd->is_nonblock = 0;
    return cmd;
}

char *stringndup(const char *str, size_t n) {
    char *rstr = malloc (sizeof (char) * n);
    sysfail (rstr==NULL, NULL);
    rstr[n] = '\0';
    strcpy (rstr, str);
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
    rqelem->q_data[0] = stringndup (cmd_str, n);
    return rqelem;
}

void pipe_list_add_cmd (cmd_line_t *cmd_line, const char *cmd_str, size_t n) {
    struct qelem *nelem = new_qelem (cmd_str, n); 
    insque (cmd_line->pipe_list_tail, nelem);
}


int parse_cmd_line (const char *cmd, cmd_line_t *cmd_line) {
    size_t prv_cmd_beg = 0, i, cmd_len;
    if (cmd_line == NULL)
        cmd_line = new_cmd_line ();
    cmd_len = strlen (cmd);

    /*assuming that the last thing is io redirection or nonblocking indicator*/
    for (i=0; i<len && !IS_IO_REDIR (cmd[i]) && !IS_NONBLOCK (cmd[i]); ++i) {
    }

} 

void release_cmd_line (cmd_line_t *cmd_line) {
    if (cmd_line != NULL)
        free (cmd_line);
}

