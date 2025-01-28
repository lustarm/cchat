#pragma once

#include "c.h"

#define MAX_CMDS 1024

typedef void (*commandfn)(client_t*);
typedef struct
{
    char name[32];
    commandfn fn;
} command;

typedef command commands_t[MAX_CMDS];
