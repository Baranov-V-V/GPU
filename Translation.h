#pragma once

#include <stdio.h>
#include <malloc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static const int MAX_NAME_LEN = 100;

static char file_in[MAX_NAME_LEN];
static char file_out[MAX_NAME_LEN];

static char current_command[MAX_COMMAND_LEN];

struct Files_data {
    char*  file_in;
    char* file_out;

    FILE* fp_in;
    FILE* fp_out;
};

void OpenTranslationFiles(struct Files_data* files_data, bool is_stdin, bool is_stdout) {
    assert(files_data != NULL);

    files_data->file_in  = (char*)calloc(MAX_NAME_LEN, sizeof(char));
    files_data->file_out = (char*)calloc(MAX_NAME_LEN, sizeof(char));

    assert(files_data->file_in  != NULL);
    assert(files_data->file_out != NULL);

    if (is_stdin == true) {
        files_data->fp_in = stdin;
        files_data->file_in = "standart input";
    }
    else {
        fprintf(stdout, "file in: ");
        fscanf(stdin, "%s", files_data->file_in);

        files_data->fp_in = fopen(files_data->file_in,  "r");
        assert(files_data->fp_in != NULL);
    }

    if (is_stdout == true) {
        files_data->fp_out = stdout;
        files_data->file_out = "standart output";
    }
    else {
    fprintf(stdout, "file out: ");
    fscanf(stdin, "%s", files_data->file_out);

    files_data->fp_out = fopen(files_data->file_out, "w");
    assert(files_data->fp_out != NULL);
    }

}

void CloseFiles(struct Files_data* files_data) {
    assert(files_data != NULL);

    fclose(files_data->fp_in);
           files_data->fp_in  = NULL;
    fclose(files_data->fp_out);
           files_data->fp_out = NULL;

    free(files_data->file_in);
         files_data->file_in  = NULL;
    free(files_data->file_out);
         files_data->file_out = NULL;
}
