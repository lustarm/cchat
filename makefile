OBJS = $(wildcard src/*.c)
NAME = cchat
DIR = build/

CC = gcc
CCFLAGS = -Wall -Wextra
CCDEFINES = -DDEBUG
CCLIB = -lpthread

all : $(OBJS)
	$(CC) $(CCFLAGS) $(OBJS) -o $(DIR)$(NAME) $(CCDEFINES)
