#include "Commands.h"
#include "Translation.h"

/*!
If Disassembly gets true as input parameter it will output in stdout
otherwise you need to put in a file to output code
!*/
void Disassembly(bool is_stdin, bool is_stdout) {
    fprintf(stdout, "Disassembling...\n");
    if (is_stdin == true) {
        fprintf(stdout, "Enter code:\n");
    }

    struct Files_data files_data;
    OpenTranslationFiles(&files_data, is_stdin, is_stdout);

    int command_no = -1;

    fscanf(files_data.fp_in, "%d", &command_no);
    while (!feof(files_data.fp_in)) {

        for (int i = 0; i < COMMANDS_COUNT; ++i) {
            if(codificator[i].code == command_no) {
                if (codificator[i].code == PUSH) {
                    float value = 0;
                    fscanf(files_data.fp_in, "%g", &value);
                    fprintf(files_data.fp_out, "%s %g\n", codificator[i].name, value);
                }
                else {
                    fprintf(files_data.fp_out, "%s\n", codificator[i].name);
                }
            }
            else {
                fprintf(stderr, "Unknown command: <%s>\n");
                exit(1);
            }
        }
        fscanf(files_data.fp_in, "%d", &command_no);
    }

    CloseFiles(&files_data);

    fprintf(stdout, "done!\n");
}


int main(int argc, char *argv[]) {
    bool is_stdin  = false;
    bool is_stdout = false;

    if (argc > 1) {

        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "--screen_in") == 0) {
                is_stdin = true;
            }
            else if (strcmp(argv[i], "--screen_out") == 0) {
                is_stdout = true;
            }
            else {
                fprintf(stderr,"Unknown command : %s\n", argv[i]);
            }
        }
    }

    Disassembly(is_stdin, is_stdout);

    return 0;
}

