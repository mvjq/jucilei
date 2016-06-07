# jucilei
jucilei

notes about libruncmd:

Signals CAN be merged, which means that if you have k child process executing, do not expect k SIGCHLD's! Can't make a test to reproduce it though :(

http://www.gnu.org/software/libc/manual/html_node/Merged-Signals.html

runcmd/test.c
Tests if runcmd supports more than one command running in background at the same time with different callback functions!
