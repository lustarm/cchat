#pragma once

#include <stdint.h>

// Add options flags later
typedef void (*atk_fn)(uint32_t id, int time, int port);

typedef struct
{
    uint32_t id;
    char name[32];
    atk_fn fn;
} atk_t;
