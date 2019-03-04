/*
    CGDraw - An instruction based CG renderer
    Copyright (C) 2019 NSKernel. All rights reserved.

    A lab of Computer Graphics at NJU

    main.c
    Main entry point of the program
 */

#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include <cgdraw.h>
#include <grapheng.h>

static const char *opt_string = "vV";
static const struct option long_opts[] = {
    { "verbose", no_argument, NULL, 'v' },
    { "version", required_argument, NULL, 'V' },
    { NULL, no_argument, NULL, 0 }
};

int main(int argc, char **argv) {
    int exec_status;
    int opt;
    int long_index;

    // parse arguments
    global_args.print_version = 0;
    global_args.verbose = 0;

    opt = getopt_long(argc, argv, opt_string, long_opts, &long_index);
    while (opt != -1) {
        switch (opt) {
            case 'V':
              global_args.print_version = 1; /* true */
              break;
            case 'v':
              global_args.verbose = 1;
            default:
              /* You won't actually get here. */
              break;
        }
        opt = getopt_long(argc, argv, opt_string, long_opts, &long_index);
    }
    
    if (argc - optind - 1 > 0) {
        global_args.instr_file_path = argv[optind];
        global_args.dest_dir = argv[optind + 1];

    }
    else {
        printf("Usage: cgdraw <instruction file path> <output path>\n");
        return -1;
    }

    if (argc - optind - 1 > 1) {
        printf("cgdraw: warning: ignoring extra arguments\n");
    }

    if (global_args.print_version) {
        if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
            printf("CGDraw version %s.%s.%s\n", VERSION, SUBVERSION, BUILD);
            printf("Copyright (C) 2019 NSKernel. All rights reserved.\n");
            return 0;
        }
    }

    // open instr file
    instr_file_pointer = fopen(global_args.instr_file_path, "r");
    if (instr_file_pointer == NULL) {
        perror("cgdraw: cannot open instruction file\n");
        return -1;
    }
    
    // set current canvas invalid because it is not initialized.
    canvas_valid = 0;

    // execute instructions
    while ((exec_status = exec_next_instr()) == EXEC_OK);
    if (exec_status == EXEC_ERROR) {
        perror("cgdraw: error occoured. exiting\n");
        return -1;
    }

    return 0;
}