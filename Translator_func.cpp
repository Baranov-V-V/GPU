#include "Commands.h"
#include "Translation.h"
#include <strings.h>
#include <string>
#include <string.h>

void OpenTranslationFiles(struct Files_data* files_data, bool is_forward) {
    assert(files_data != NULL);

    files_data->file_in  = (char*)calloc(MAX_NAME_LEN, sizeof(char));
    files_data->file_out = (char*)calloc(MAX_NAME_LEN, sizeof(char));

    assert(files_data->file_in  != NULL);
    assert(files_data->file_out != NULL);

    //files_data->file_in = "asm_code.txt";
    //files_data->file_out = "out.bin";
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

void Change_Char(char* text, size_t text_size, char old_c, char new_c) {
    assert(text != NULL);
    assert(text_size >= 0);

    int i = 0;
    for (i; i < text_size; i++) {
        if (text[i] == old_c) {
            text[i] = new_c;
        }
    }
}

char* ReadToBuffer(Files_data* files_data, int* buf_size) {
    char* buffer = (char*) calloc(files_data->input_length + 1, sizeof(char));

    *buf_size = fread(buffer, sizeof(char), files_data->input_length, files_data->fp_in);

    buffer[*buf_size] = ' ';

    Change_Char(buffer, *buf_size, '\n', ' ');

    return buffer;
}

void FindLabel(struct AsmInfo* asm_info, struct LabelsInfo* labels_info) {
    bool is_found = false;
    for(int i = 0; i < labels_info->labels_count; ++i) {
        if (strcmp(asm_info->current_command, labels_info->labels[i].name) == 0) {
            *(int*) (asm_info->code_file + asm_info->pos)= labels_info->labels[i].pos;
            is_found = true;
            asm_info->pos += sizeof(int);
            break;
        }
    }

    if (is_found == false) {
        fprintf(stderr,"syntax error: can't find label with name: <%s>", asm_info->current_command);
        exit(1);
    }
}

void MarkJumpPos(struct AsmInfo* asm_info, int jump_no) {
    asm_info->code_file[asm_info->pos++] = codificator[jump_no].code;
    *(int*) (asm_info->code_file + asm_info->pos)= -1;
    asm_info->pos += sizeof(int);
    GetNewAsmCommand(asm_info); // пропускаем метку
}

void GetNewAsmCommand(struct AsmInfo* asm_info) {
    int current_symbols_read = 0;
    sscanf(asm_info->buffer + asm_info->symbols_read, "%s %n", asm_info->current_command, &current_symbols_read);
    asm_info->symbols_read += current_symbols_read;
}

void GetNewString(struct AsmInfo* asm_info, char* tmp_command) {
    int current_symbols_read = 0;
    sscanf(asm_info->buffer + asm_info->symbols_read, "%s %n", tmp_command, &current_symbols_read);
    asm_info->symbols_read += current_symbols_read;
}

void GetPop(struct AsmInfo* asm_info) {
    char pop_index = 25;
    char* pop_value = (char*) calloc(MAX_COMMAND_LEN, sizeof(char));
    struct Push_values pop_values;

    char tmp_command[MAX_COMMAND_LEN];
    sscanf(asm_info->buffer + asm_info->symbols_read, "%s", tmp_command);

    if(tmp_command[0] != '[' && tmp_command[0] != 'r') {
        ProceedPushPop(asm_info, &pop_index, pop_value, &pop_values);
        return;
    }

    GetNewString(asm_info, pop_value);

    ProceedPushPop(asm_info, &pop_index, pop_value, &pop_values);

    MakePushPop(asm_info, &pop_values, pop_index);

    free(pop_value);
}

int stricmp (const char *s1, const char *s2) {
    while (*s1 != 0 && *s2 != 0) {
        if (*s1 != *s2 && toupper(*s1) != toupper(*s2))
        {
            return -1;
        }
        s1++;
        s2++;
    }
    return (*s1 == 0 && *s2 == 0) ? 0 : -1;
}

void MakePushPop(struct AsmInfo* asm_info, struct Push_values* push_values, char push_index) {
    asm_info->code_file[asm_info->pos++] = push_index;

    if ((push_index >> 6) % 2 != 0) {
        asm_info->code_file[asm_info->pos++] = push_values->rx_type;
    }
    if ((push_index >> 5) % 2 != 0) {
        *(type_t*) (asm_info->code_file + asm_info->pos) = push_values->value;
        asm_info->pos += sizeof(type_t);
    }
}

void PushPop128(char* push_index, char** push_value) {
    *push_index = *push_index | 128;
    *push_value = *push_value + 1;
}

void PushPop64(char* push_index, char* push_value, struct Push_values* push_values, struct AsmInfo* asm_info) {

    *push_index = *push_index | 64;

    if (push_value[strlen(push_value) - 1] == ']') {
        push_value[strlen(push_value) - 1] = '\0';
    }

    #define DEF_REG(name_upper, name_lower, num, code)    \
    if (strcmp(push_value, #name_lower) == 0) {           \
        push_values->rx_type = name_upper;                \
    }

    #include "Registers_Gen.h"

    #undef DEF_REG

    char tmp_command[MAX_COMMAND_LEN];
    int current_symbols_read = 0;
    sscanf(asm_info->buffer + asm_info->symbols_read, "%s %n", tmp_command, &current_symbols_read);

    if (tmp_command[0] == '+') {
        asm_info->symbols_read += current_symbols_read;
        GetNewString(asm_info, push_value);
    }
}

void PushPop32(char* push_index, char* push_value, struct Push_values* push_values) {
    *push_index = *push_index | 32;

    if ((*push_index >> 7) % 2 == 1) {
        push_value[strlen(push_value) - 1] = '\0';
    }

    push_values->value = atof(push_value);
}

void ProceedPushPop(struct AsmInfo* asm_info, char* push_index, char* push_value, struct Push_values* push_values) {
    if (push_value[0] == '[') {
        PushPop128(push_index, &push_value);
    }

    if (push_value[0] == 'r') {
        PushPop64(push_index, push_value, push_values, asm_info);
    }

    if (isdigit(push_value[0])) {
        PushPop32(push_index, push_value, push_values);
    }

    if (*push_index == 24) {
        printf("Syntax error: can't find arguments for push\n");
        exit(1);
    }

}

void GetPush(struct AsmInfo* asm_info) {
    char push_index = 24;
    char* push_value = (char*) calloc(MAX_COMMAND_LEN, sizeof(char));

    GetNewString(asm_info, push_value);

    struct Push_values push_values;

    ProceedPushPop(asm_info, &push_index, push_value, &push_values);

    MakePushPop(asm_info, &push_values, push_index);

    free(push_value);
}

void GetJmp(struct AsmInfo* asm_info, struct LabelsInfo* labels_info, int jump_no) {
    if (asm_info->asm_number == 1) {
        MarkJumpPos(asm_info, jump_no);
    }
    else if (asm_info->asm_number == 2) {
        GetNewAsmCommand(asm_info);
        asm_info->pos++;
        FindLabel(asm_info, labels_info);
    }
}

void EmplaceLabel(struct LabelsInfo* labels_info, struct AsmInfo* asm_info) {
    Label new_lable;
    asm_info->current_command[strlen(asm_info->current_command) - 1] = '\0';
    strcpy(new_lable.name, asm_info->current_command);
    new_lable.pos = asm_info->pos - sizeof(FileHeader);

    labels_info->labels[labels_info->labels_count++] = new_lable;
}

void GetCommands(struct AsmInfo* asm_info, struct LabelsInfo* labels_info) {
    bool isfound = false;
    for (int i = 0; i < COMMANDS_COUNT; ++i) {
        if (stricmp(asm_info->current_command, codificator[i].name) == 0) {
            isfound = true;
            if (codificator[i].code == COMMAND_PUSH) {
                GetPush(asm_info);
            }
            else if (codificator[i].code == COMMAND_POP) {
                GetPop(asm_info);
            }
            else if (i >= BEGIN_JMP_CODES && i <= END_JMP_CODES) {
                GetJmp(asm_info, labels_info, i);
            }
            else {
                asm_info->code_file[asm_info->pos++] = codificator[i].code;
            }
            break;
        }
    }
    if (isfound == false) {
        printf("Syntax error: unknown command with name <%s>\n", asm_info->current_command);
        exit(1);
    }
}

void MakeAsmInfo(struct AsmInfo* asm_info, struct Files_data* files_data) {
    int pos = 0;
    char* code_file = ConstructCodeFile(files_data, 1, &pos);

    int buf_size = 0;
    char* buffer = ReadToBuffer(files_data, &buf_size);

    asm_info->buffer = buffer;
    asm_info->buf_size = buf_size;
    asm_info->pos = pos;
    asm_info->code_file = code_file;
    asm_info->commands_counter = 0;
    asm_info->symbols_read = 0;
    asm_info->asm_number = 1;
}

void ChangeAsmNumber(struct AsmInfo* asm_info, int asm_number) {
    asm_info->pos = sizeof(FileHeader);
    asm_info->commands_counter = 0;
    asm_info->symbols_read = 0;
    asm_info->commands_counter = 0;
    asm_info->asm_number = asm_number;
}

void Assembly(struct AsmInfo* asm_info, struct LabelsInfo* labels_info) {
    while (asm_info->symbols_read < asm_info->buf_size) {
        asm_info->commands_counter++;
        GetNewAsmCommand(asm_info);
        if (asm_info->current_command[strlen(asm_info->current_command) - 1] == ':') {
            EmplaceLabel(labels_info, asm_info);
            continue;
        }

        GetCommands(asm_info, labels_info);
    }
}
