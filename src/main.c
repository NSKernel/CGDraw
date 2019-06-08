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
#include <sys/stat.h>

#include <cgdraw.h>
#include <grapheng.h>
#include <gui_gen.h>

static const char *opt_string = "vg:iV";
static const struct option long_opts[] = {
    { "verbose", no_argument, NULL, 'v' },
    { "version", no_argument, NULL, 'V' },
    { "interactive", no_argument, NULL, 'i' },
    { "gui-files", required_argument, NULL, 'g' },
    { NULL, no_argument, NULL, 0 }
};

int main(int argc, char **argv) {
    int exec_status;
    int opt;
    int long_index;

    // parse arguments
    global_args.print_version = 0;
    global_args.verbose = 0;
    global_args.gui_enabled = 0;

    opt = getopt_long(argc, argv, opt_string, long_opts, &long_index);
    while (opt != -1) {
        switch (opt) {
            case 'V':
              global_args.print_version = 1; /* true */
              break;
            case 'v':
              global_args.verbose = 1;
              break;
            case 'g':
              global_args.gui_enabled = 1;
              global_args.gui_file_path = optarg;
              break;
            case '?':
              // getopt_long will report problem by itself
              return -1;
            default:
              /* You won't actually get here. */
              break;
        }
        opt = getopt_long(argc, argv, opt_string, long_opts, &long_index);
    }
    
    if (global_args.print_version) {
        printf("CGDraw version %s.%s.%s\n", VERSION, SUBVERSION, BUILD);
        printf("Copyright (C) 2019 NSKernel. All rights reserved.\n");
        return 0;
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

    // open instr file
    instr_file_pointer = fopen(global_args.instr_file_path, "r");
    if (instr_file_pointer == NULL) {
        perror("cgdraw: cannot open instruction file\n");
        return -1;
    }

    struct stat st;
    if (stat(global_args.dest_dir, &st) != 0) {
        mkdir(global_args.dest_dir, 0777);
    }
    
    // set current canvas invalid because it is not initialized.
    canvas_valid = 0;

    // execute instructions
    while ((exec_status = exec_next_instr()) == EXEC_OK);
    if (exec_status == EXEC_ERROR) {
        perror("cgdraw: error occoured. exiting\n");
        return -1;
    }
    // gracefully exited
    // now check if gui is enabled
    if (global_args.gui_enabled) {
        if (gui_save(global_args.gui_file_path) != EXEC_OK) {
            perror("cgdraw: error occoured. exiting\n");
            return -1;
        }
    }

    return 0;
}