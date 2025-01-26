OBJS = $(wildcard src/*.c)
NAME = cchat
DIR = build/

CC = gcc
CCFLAGS = -Wall -Wextra
CCDEFINES = -DDEBUG -DLOG_USE_COLOR
CCLIB = -lpthread

all : $(OBJS)
	$(CC) $(CCFLAGS) $(OBJS) -o $(DIR)$(NAME) $(CCDEFINES)
