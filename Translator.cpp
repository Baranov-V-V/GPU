#include "Commands.h"
#include "Translation.h"

/*!
If assembly gets true as input parameter it will scan stdin for data
otherwise you need to put in a file to scan for data
!*/
void Assembly() {
    fprintf(stdout, "Assembling...\n");

    struct Files_data files_data;
    OpenTranslationFiles(&files_data, true);

    int pos = 0;
    char* code_file = ConstructCodeFile(&files_data, 1, &pos);
    printf("%d\n", code_file[3]);

    int commands_counter = 0;
    bool skip = false;

    while (!feof(files_data.fp_in) || skip == true) {
        ++commands_counter;

        if (skip == false) {
            fscanf(files_data.fp_in, "%s", current_command);
        }
        skip = false;

        printf("pos: %d, command: %s\n", pos, current_command);

        for (int i = 0; i < COMMANDS_COUNT; ++i) {
            if (strcmp(current_command, codificator[i].name) == 0) {
                if (codificator[i].code == PUSH) {
                    float value = 0;
                    int count = fscanf(files_data.fp_in, "%g", &value);
                    if (count == 0) {
                        char rx[MAX_COMMAND_LEN];
                        fscanf(files_data.fp_in, "%s", rx);

                        code_file[pos++] = REG_PUSH;

                        if (strcmp(rx, "rax") == 0) {
                            code_file[pos++] = 1;
                        }
                        if (strcmp(rx, "rbx") == 0) {
                            code_file[pos++] = 2;
                        }
                        if (strcmp(rx, "rcx") == 0) {
                            code_file[pos++] = 3;
                        }
                        if (strcmp(rx, "rdx") == 0) {
                            code_file[pos++] = 4;
                        }
                    }
                    else {
                        code_file[pos++] = PUSH;
                        *(type_t*) (code_file + pos) = value;
                        pos += sizeof(type_t);
                    }
                }
                else if (codificator[i].code == POP) {
                    char tmp_command[MAX_COMMAND_LEN];
                    fscanf(files_data.fp_in, "%s", tmp_command);

                    if (strcmp(tmp_command, "rax") == 0) {
                        code_file[pos++] = REG_POP;
                        code_file[pos++] = 1;
                    }
                    else if (strcmp(tmp_command, "rbx") == 0) {
                        code_file[pos++] = REG_POP;
                        code_file[pos++] = 2;
                    }
                    else if (strcmp(tmp_command, "rcx") == 0) {
                        code_file[pos++] = REG_POP;
                        code_file[pos++] = 3;
                    }
                    else if (strcmp(tmp_command, "rdx") == 0) {
                        code_file[pos++] = REG_POP;
                        code_file[pos++] = 4;
                    }
                    else {
                        code_file[pos++] = POP;
                        strcpy(current_command, tmp_command);
                        skip = true;
                    }
                }
                else {
                    code_file[pos++] = codificator[i].code;
                }

            break;
            }

        }
    }

    UpdateCodeFile(code_file, commands_counter, pos);

    fwrite(code_file, pos, 1, files_data.fp_out);

    free(code_file);
         code_file = NULL;

    CloseFiles(&files_data);

    fprintf(stdout, "done!\n");
}


int main(int argc, char *argv[]) {

    Assembly();

    return 0;
}
