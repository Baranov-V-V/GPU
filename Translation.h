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

struct AsmInfo {
    char* buffer;
    char* code_file;
    char current_command[MAX_COMMAND_LEN];

    int symbols_read;
    int pos;
    int asm_number;
    int commands_counter;
    int buf_size;
};

struct LabelsInfo {
    Label labels[MAX_LABELS_COUNT];
    int labels_count;
};


char* ReadToBuffer(Files_data* files_data, int* buf_size);

void Change_Char(char* text, size_t text_size, char old_c, char new_c);

char* UnpackFile(Files_data* files_data, int* pos, int* commands_count);

void UpdateCodeFile(char* code_file, int commands_count, int pos);

char* ConstructCodeFile(Files_data* files_data, int current_version, int* file_pos);

void CloseFiles(struct Files_data* files_data);

void OpenTranslationFiles(struct Files_data* files_data, bool is_forward);

void FindLabel(struct AsmInfo* asm_info, struct LabelsInfo* labels_info);

void MarkJumpPos(struct AsmInfo* asm_info, int jump_no);

void GetNewAsmCommand(struct AsmInfo* asm_info);

void GetNewString(struct AsmInfo* asm_info, char* tmp_command);

void GetPop(struct AsmInfo* asm_info);

void GetRegPush(struct AsmInfo* asm_info, char* register_name);

void GetValuePush(struct AsmInfo* asm_info, char* push_value);

void GetPush(struct AsmInfo* asm_info);

void GetJmp(struct AsmInfo* asm_info, struct LabelsInfo* labels_info, int jump_no);

void EmplaceLabel(struct LabelsInfo* labels_info, struct AsmInfo* asm_info);

void GetCommands(struct AsmInfo* asm_info, struct LabelsInfo* labels_info);

void MakeAsmInfo(struct AsmInfo* asm_info, struct Files_data* files_data);

void ChangeAsmNumber(struct AsmInfo* asm_info, int asm_number);

void Assembly(struct AsmInfo* asm_info, struct LabelsInfo* labels_info);

void ProceedPushPop(struct AsmInfo* asm_info, char* push_index, char* push_value, struct Push_values* push_values);

void MakePushPop(struct AsmInfo* asm_info, struct Push_values* push_values, char push_index);

void PushPop128(char* push_index, char** push_value);

void PushPop64(char* push_index, char* push_value, struct Push_values* push_values, struct AsmInfo* asm_info);

void PushPop32(char* push_index, char* push_value, struct Push_values* push_values);
