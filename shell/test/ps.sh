PS_COMMAND='ps o tty,comm:35,pgrp,pid,session,stat --forest'

clear;

while true; 
do 
    $PS_COMMAND; 
    sleep 1; 
    clear; 
done;
