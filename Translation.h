#pragma once

#include <stdio.h>
#include <malloc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys\stat.h>
#include "Stack.h"

static const int MAX_NAME_LEN = 100;

static char file_in[MAX_NAME_LEN];
static char file_out[MAX_NAME_LEN];

static char current_command[MAX_COMMAND_LEN];

struct Files_data {
    char*  file_in;
    char* file_out;

    FILE* fp_in;
    FILE* fp_out;

    int input_length;
};

// forward отвечает за то что мы ассемблируем(тру) или дизассемблируем(фалсе)
void OpenTranslationFiles(struct Files_data* files_data, bool is_forward) {
    assert(files_data != NULL);

    files_data->file_in  = (char*)calloc(MAX_NAME_LEN, sizeof(char));
    files_data->file_out = (char*)calloc(MAX_NAME_LEN, sizeof(char));

    assert(files_data->file_in  != NULL);
    assert(files_data->file_out != NULL);


    fprintf(stdout, "file in: ");
    fscanf(stdin, "%s", files_data->file_in);

    if (is_forward == true) {
        files_data->fp_in = fopen(files_data->file_in,  "r");
    }
    else {
        files_data->fp_in = fopen(files_data->file_in,  "rb");
    }
    assert(files_data->fp_in != NULL);

    fprintf(stdout, "file out: ");
    fscanf(stdin, "%s", files_data->file_out);

    if (is_forward == true) {
        files_data->fp_out = fopen(files_data->file_out, "wb");
    }
    else {
        files_data->fp_out = fopen(files_data->file_out, "w");
    }
    assert(files_data->fp_out != NULL);

    struct stat info;
    stat(files_data->file_in, &info);
    files_data->input_length = info.st_size;
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

char* ConstructCodeFile(Files_data* files_data, int current_version, int* file_pos) {
    char* code_file = (char*) calloc(sizeof(FileHeader) + files_data->input_length * (sizeof(char) + sizeof(type_t)), 1);//домножаем как будто у нас все пуши

    FileHeader* file_header = (FileHeader*) code_file;

    file_header->initials[0] = 'B';
    file_header->initials[1] = 'V';
    file_header->initials[2] = 'V';

    file_header->version = current_version;
    file_header->commands_count = 0;

    *file_pos = sizeof(FileHeader);

    //printf("%c%c%c\n",code_file[0],code_file[1],code_file[2]);

    return code_file;
}

void UpdateCodeFile(char* code_file, int commands_count, int pos) {
    FileHeader* file_header = (FileHeader*) code_file;

    file_header->commands_count = commands_count;
    file_header->code_size = pos - sizeof(FileHeader);
}

char* UnpackFile(Files_data* files_data, int* pos, int* commands_count) {
    char* code_file = (char*) calloc(files_data->input_length, sizeof(char));
    fread(code_file, files_data->input_length, 1, files_data->fp_in);

    printf("input len: %d", files_data->input_length);

    FileHeader* file_header = (FileHeader*) code_file;

    fprintf(files_data->fp_out,"Current version: %hhd\n", file_header->version);
    fprintf(files_data->fp_out,"Total commands count: %d\n", file_header->commands_count);
    fprintf(files_data->fp_out,"Commands code size in bytes: %d\n", file_header->code_size);

    *pos = sizeof(FileHeader);
    *commands_count = file_header->commands_count;

    return code_file;
}
