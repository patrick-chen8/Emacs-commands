#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define BUFFSIZE 4096

/**
 * A recreation of the wc command in unix.
 */
int main(int argc, char *argv []) {

    // defining variables
    char text[BUFFSIZE];
    int m;
    bool c = false;
    bool l = false;
    bool w = false;
    int bytes;
    int lines;
    int words;
    int totalbytes = 0;
    int totallines = 0;
    int totalwords = 0;
    int oflags = O_RDONLY;
    int fd = 0;
    int arg;
    int num = 0;

    setbuf(stdout, NULL);

    // loop that runs through the command line arguments
    for (arg = 1; arg < argc; arg ++) {
        bytes = 0;
        lines = 0;
        words = 0;

        // reading if c is an option
        if (strcmp(argv[arg], "-c") == 0) {
            c = true;

            // reading if l is an option
        } else if (strcmp(argv[arg], "-l") == 0) {
            l = true;

            // reading dash option
        } else if (strcmp(argv[arg], "-w") == 0) {
            w = true;

        } else if (strcmp(argv[arg], "-cl") == 0) {
            c = true;
            l = true;

        } else if (strcmp(argv[arg], "-cw") == 0) {
            c = true;
            w = true;

        } else if (strcmp(argv[arg], "-lc") == 0) {
            l = true;
            c = true;

        } else if (strcmp(argv[arg], "-lw") == 0) {
            l = true;
            w = true;

        } else if (strcmp(argv[arg], "-wc") == 0) {
            w = true;
            c = true;

        } else if (strcmp(argv[arg], "-wl") == 0) {
            w = true;
            l = true;

        } else if (strcmp(argv[arg], "-wlc") == 0) {
            w = true;
            l = true;
            c = true;

        } else if (strcmp(argv[arg], "-wcl") == 0) {
            w = true;
            l = true;
            c = true;

        } else if (strcmp(argv[arg], "-lwc") == 0) {
            w = true;
            l = true;
            c = true;

        } else if (strcmp(argv[arg], "-lcw") == 0) {
            w = true;
            l = true;
            c = true;

        } else if (strcmp(argv[arg], "-cwl") == 0) {
            w = true;
            l = true;
            c = true;

        } else if (strcmp(argv[arg], "-clw") == 0) {
            w = true;
            l = true;
            c = true;

            // reading std input
        } else if (*argv[arg] == '-') {
            if (arg == 1 && w == false && l == false && c == false) {
                w = true;
                l = true;
                c = true;
            }
            fd = -1;
            int total = 0;
            int space;
            while ((m = read(STDIN_FILENO, text, BUFFSIZE)) > 0) {
                if (m == -1) {
                    perror("read");
                }
                for (int i = 0; i < m; i++) {
                    space = isspace(text[i]);
                    if (text[i] == '\n') {
                        lines++;
                    } else if (space == 0) {
                        space = isspace(text[i + 1]);
                        if (space != 0) {
                            words++;
                        }
                    }
                }
                total += m;
            }
            bytes = total;

            totallines += lines;
            totalwords += words;
            totalbytes += bytes;
            num += 1;

            // printing results
            if (l == true) {
                printf("%d\t", lines);
                if (w == true) {
                    printf("%d\t", words);
                    if (c == true) {
                        printf("%d\t", bytes);
                    }
                }
            }
            printf("\t-\n");

            // reading files
        } else if ((fd = open(argv[arg], oflags)) > 0) {
            if (fd == -1) {
                perror("open");
            }

            if (arg == 1 && w == false && l == false && c == false) {
                w = true;
                l = true;
                c = true;
            }

            m = read(fd, text, BUFFSIZE);
            if (m == -1) {
                perror("read");
            }

            int space;
            bytes = m;
            for (int i = 0; i < m; i++) {
                space = isspace(text[i]);
                if (text[i] == '\n') {
                    lines++;
                } else if (space == 0) {
                    space = isspace(text[i + 1]);
                    if (space != 0) {
                        words++;
                    }
                }
            }

            totallines += lines;
            totalwords += words;
            totalbytes += bytes;
            num += 1;

            // printing results
            if (l == true) {
                printf("%d\t", lines);
                if (w == true) {
                    printf("%d\t", words);
                    if (c == true) {
                        printf("%d\t", bytes);
                    }
                }
            }
            printf("\t");
            printf(argv[arg]);
            printf("\n");
        }
    }

    // handles no file
    if (argc == 1 || fd == 0) {

        lines = 0;
        words = 0;
        bytes = 0;

        if (arg == 1 && w == false && l == false && c == false) {
            w = true;
            l = true;
            c = true;
        }
        fd = -1;
        int total = 0;
        int space;
        while ((m = read(STDIN_FILENO, text, BUFFSIZE)) > 0) {
            if (m == -1) {
                perror("read");
            }
            for (int i = 0; i < m; i++) {
                space = isspace(text[i]);
                if (text[i] == '\n') {
                    lines++;
                } else if (space == 0) {
                    space = isspace(text[i + 1]);
                    if (space != 0) {
                        words++;
                    }
                }
            }
            total += m;
        }
        bytes = total;

        totallines += lines;
        totalwords += words;
        totalbytes += bytes;
        num += 1;

        // printing results
        if (l == true) {
            printf("%d\t", lines);
            if (w == true) {
                printf("%d\t", words);
                if (c == true) {
                    printf("%d\t", bytes);
                }
            }
        }
        printf("\n");
    }

    if (num >= 2) {
        if (l == true) {
            printf("%d\t", totallines);
            if (w == true) {
                printf("%d\t", totalwords);
                if (c == true) {
                    printf("%d\t", totalbytes);
                }
            }
        }
        printf("\ttotal\n");
    }

} // main
