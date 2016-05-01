#include <runcmd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

    char cmd[1024];
    int res, io[2];

    fgets(cmd, 1024, stdin);

    res=0;
    runcmd(cmd, &res, io);

    return 0;
}
