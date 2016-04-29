#ifndef RUNCMD_H
#define RUNCMD_H

#define RCMD_MAXARGS 1024
#define RCMD_DELIM "\n\t\r" 
#define RCMD_MAXARGS 1024

#define NORTERM (1<<8)
#define EXECOK (1<<9)
#define NONBLOCK (1<<10)
#define RETSTATUS (0xFF)
#define EXITFAILSTATUS (127)

#define IS_NORTERM(res) ((res & NORTERM))
#define IS_NONBLOCK(res) ((res & NONBLOCK))
#define EXITSTATUS(res) ((res & RETSTATUS))
#define IS_EXECOK(res) ((res & EXECOK))

int runcmd(const char *command, int *result, int *io);

extern void (*runcmd_onexit)(void);

#endif
