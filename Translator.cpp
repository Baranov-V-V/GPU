#include "Commands.h"
#include "Translation.h"

/*!
If assembly gets true as input parameter it will scan stdin for data
otherwise you need to put in a file to scan for data
!*/
void Assembly(bool is_stdin, bool is_stdout) {
    fprintf(stdout, "Assembling...\n");
    if (is_stdin == true) {
        fprintf(stdout, "Enter code:\n");
    }

    struct Files_data files_data;
    OpenTranslationFiles(&files_data, is_stdin, is_stdout);

    while (!feof(files_data.fp_in)) {
        fscanf(files_data.fp_in, "%s", current_command);
        for (int i = 0; i < COMMANDS_COUNT; ++i) {
            if (strcmp(current_command, codificator[i].name) == 0) {
                if (codificator[i].code == PUSH) {
                    float value = 0;
                    fscanf(files_data.fp_in, "%g", &value);
                    fprintf(files_data.fp_out, "%d %g\n", codificator[i].code, value);
                }
                else {
                    fprintf(files_data.fp_out, "%d\n", codificator[i].code);
                }
            }
            //else {
              //  fprintf(stderr, "Unknown command: <%s>\n");
               // exit(1);
            //}
        }
        fprintf(stderr, "%s\n", current_command);
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

    Assembly(is_stdin, is_stdout);

    return 0;
}
