#include <stdio.h>
#include <malloc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static const int MAX_NAME_LEN = 100;
static const int MAX_COMMAND_LEN = 10;

static char file_in[MAX_NAME_LEN];
static char file_out[MAX_NAME_LEN];

static char current_command[MAX_COMMAND_LEN];


