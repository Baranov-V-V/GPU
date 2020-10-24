#include "Commands.h"
#include "Translation.h"

/*!
If Disassembly gets true as input parameter it will output in stdout
otherwise you need to put in a file to output code
!*/
void Disassembly() {
    fprintf(stdout, "Disassembling...\n");

    struct Files_data files_data;
    OpenTranslationFiles(&files_data, false);

    int pos = 0;
    int commands_count = 0;

    char* code_file = UnpackFile(&files_data, &pos, &commands_count);

    printf("pos: %d, count: %d\n", pos, commands_count);

    for (int i = 0; i < commands_count; ++i) {
        char command_no = code_file[pos++];
        printf("pos: %d\n", pos);

        for (int j = 0; j < COMMANDS_COUNT; ++j) {
            if (codificator[j].code == command_no) {
                if (codificator[j].code == PUSH) {
                    fprintf(files_data.fp_out, "%s ", codificator[j].name);
                    fprintf(files_data.fp_out, "%g\n", *(type_t*) (code_file + pos));
                    pos += sizeof(type_t);
                }
                else if (codificator[j].code == REG_PUSH || codificator[j].code == REG_POP) {
                    fprintf(files_data.fp_out, "%s ", codificator[j].name);
                    int register_position = code_file[pos++];
                    switch (register_position) {

                        case RAX: fprintf(files_data.fp_out, "rax\n");
                            break;
                        case RBX: fprintf(files_data.fp_out, "rbx\n");
                            break;
                        case RCX: fprintf(files_data.fp_out, "rcx\n");
                            break;
                        case RDX: fprintf(files_data.fp_out, "rdx\n");
                            break;
                        default : fprintf(stderr,"Register with pos <%d> doesn't exist", register_position);
                    }
                }
                else {
                    fprintf(files_data.fp_out, "%s\n", codificator[j].name);
                }
            break;
            }
        }
    }

    free(code_file);
         code_file = NULL;

    CloseFiles(&files_data);

    fprintf(stdout, "done!\n");
}


int main(int argc, char *argv[]) {

    Disassembly();

    return 0;
}

