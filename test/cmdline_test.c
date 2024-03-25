#include <stdio.h>
#include <string.h>

#define MAX_ARGS 10

int parse_command_line(int argc, char *argv[], char *options[], char *values[]) {
    int i, num_options = 0;

    for (i = 1; i < argc && num_options < MAX_ARGS; i++) {
        if (argv[i][0] == '-') {
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                options[num_options] = &argv[i][1];
                values[num_options++] = argv[i + 1];
                i++;
            } else {
                options[num_options] = &argv[i][1];
                values[num_options++] = NULL;
            }
        } else {
            fprintf(stderr, "Invalid argument: %s\n", argv[i]);
            return -1;
        }
    }

    return num_options;
}

int main(int argc, char *argv[]) {
    char *options[MAX_ARGS];
    char *values[MAX_ARGS];
    int num_options = parse_command_line(argc, argv, options, values);

    if (num_options == -1) {
        return 1;
    }

    for (int i = 0; i < num_options; i++) {
        if (values[i]) printf("Option: %s, Value: %s\n", options[i], values[i]);
        else printf("Option: %s with no value", options[i]);
    }
    return 0;
}