PROJECT = runcmd
PREFIX = ./local

CC = gcc
CPP_FLAGS = -Wall --ansi --pedantic-errors -D_POSIX_C_SOURCE=200809L
C_FLAGS =
LD_FLAGS =
MAKE = make
AR = ar
TAR = tar 

bin = 
lib = libruncmd
libruncmd_obj = runcmd.o
libruncmd_h = runcmd.h

EXTRA_DIST = Makefile config.mk
