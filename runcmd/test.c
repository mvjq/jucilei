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
