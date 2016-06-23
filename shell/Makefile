#    Makefile, Copyright 2016 Danilo Tedeschi <danfyty@gmail.com>
#    Makefile is derived from posixeg, Copyright 2014  Monaco F. J.   <monaco@icmc.usp.br>
#
#    This file is part of Jucilei
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with .  If not, see <http://www.gnu.org/licenses/>. 


#TODO:       migrate to autotools ####
#   make distcheck
#   make check

include config.mk

#parser: parser.h parser.o
#	$(CC) $(CPP_FLAGS) $(CFLAGS) $(C_FLAGS) parser.o -o parser -g
parser.o: parser.c
	$(CC) $(CPP_FLAGS) -c $(C_FLAGS) parser.c -o parser.o

process.o: process.c
	$(CC) $(CPP_FLAGS) -c $(C_FLAGS) process.c -o process.o

job.o: job.h job.c
	$(CC) $(CPP_FLAGS) -c $(C_FLAGS) job.c -o job.o

shell.o: shell.c shell.h
	$(CC) $(CPP_FLAGS) -c $(C_FLAGS) shell.c -o shell.o

main: parser.o process.o job.o shell.o main.c 
	$(CC) $(CPP_FLAGS) $(C_FLAGS) parser.o job.o process.o shell.o main.c -o main

.PONHY: clean

clean:
	rm -f parser.o
	rm -f main
	rm -f process.o
	rm -f shell.o
	rm -f job.o
