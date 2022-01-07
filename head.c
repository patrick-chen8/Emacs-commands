#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define BUFFSIZE 4096

/**
 * A recreation of the head command in unix.
 */
int main(int argc, char *argv []) {

    // defining variables
    char text[BUFFSIZE];
    int m;
    int c_bytes = 0;
    int n_lines = 0;
    int oflags = O_RDONLY;
    int fd = 0;
    int count = 0;
    int newlines =  0;
    int arg;

    setbuf(stdout, NULL);

    // loop that runs through the command line arguments
    for (arg = 1; arg < argc; arg++) {

        // reading the number following n
        if (strcmp(argv[arg], "-n") == 0) {
            n_lines = atoi(argv[++arg]);
            if (n_lines <= 0) {
                exit(1);
            }

            // reading the number following c
        } else if (strcmp(argv[arg], "-c") == 0) {
            c_bytes = atoi(argv[++arg]);
            if (c_bytes <= 0) {
                exit(1);
            }

            // reading dash option
        } else if (*argv[arg] == '-') {
            if (fd > 0) {
                printf("\n==> standard input <==\n");
            } else if (fd == 0 && argc >= 5) {
                printf("==> standard input <==\n");
            }
            fd = -1;

            // considering all options with c and n
            if (n_lines == 0 && c_bytes == 0) {
                n_lines = 10;
                for (int i = 0; i < n_lines; i++) {
                    m = read(STDIN_FILENO, text, BUFFSIZE);
                    if (m == -1) {
                        perror("read");
                    }
                    m = write(STDOUT_FILENO, text, m);
                    if (m == -1) {
                        perror("write");
                    }
                }

            } else if (n_lines == 0 && c_bytes != 0) {
                int byteCount = 0;
                while (byteCount < c_bytes) {
                    m = read(STDIN_FILENO, text, BUFFSIZE);
                    if (m == -1) {
                        perror("read");
                    }
                    byteCount += m;
                }

                if (byteCount <= c_bytes) {
                    m = write(STDOUT_FILENO, text, m);
                    if (m == -1) {
                        perror("write");
                    }
                } else {
                    m = write(STDOUT_FILENO, text, m - (byteCount - c_bytes));
                    if (m == -1) {
                        perror("write");
                    }
                }

            } else if (n_lines != 0 && c_bytes == 0) {
                for (int i = 0; i < n_lines; i++) {
                    m = read(STDIN_FILENO, text, BUFFSIZE);
                    if (m == -1) {
                        perror("read");
                    }
                    m = write(STDOUT_FILENO, text, m);
                    if (m == -1) {
                        perror("write");
                    }
                }

            }

            // reading files
        } else if ((fd = open(argv[arg], oflags)) > 0) {
            if (argc >= 5 && arg == 3) {
                printf("==> ");
                printf(argv[arg]);
                printf(" <==\n");
            } else if (argc >= 5 && arg > 3) {
                printf("\n==> ");
                printf(argv[arg]);
                printf(" <==\n");
            }
            if (fd == -1) {
                perror("open");
            }

            count = 0;
            newlines =  0;

            // considering all options with n and c
            if (n_lines == 0 && c_bytes == 0) {
                n_lines = 10;
                m = read(fd, text, BUFFSIZE);
                if (m == -1) {
                    perror("read");
                }

                for (int i = 0; i < m; i++) {
                    if (text[i] == '\n') {
                        newlines++;
                        if (newlines == n_lines) {
                            count = i + 1;
                            break;
                        }
                    }
                }
                m = write(STDOUT_FILENO, text, count);
                if (m == -1) {
                    perror("write");
                }

            } else if (n_lines != 0 && c_bytes == 0) {
                m = read(fd, text, BUFFSIZE);
                if (m == -1) {
                    perror("read");
                }

                for (int i = 0; i < m; i++) {
                    if (text[i] == '\n') {
                        newlines++;
                        if (newlines == n_lines) {
                            count = i + 1;
                            break;
                        }
                    }
                }
                if (newlines < n_lines) {
                    count = m;
                }
                m = write(STDOUT_FILENO, text, count);
                if (m == -1) {
                    perror("write");
                }

            } else if (n_lines == 0 && c_bytes != 0) {
                if ((m = read(fd, text, c_bytes)) > 0) {
                    if (write(STDOUT_FILENO, text, m) !=m) {
                        perror("write");
                    }
                } else if (m == -1) {
                    perror("read");
                }
            }
        }
    }

    // if there are no files in the command line argument
    if (argc == 1 || ((c_bytes != 0 || n_lines !=0) && fd == 0)) {
        if (n_lines == 0 && c_bytes == 0) {
            n_lines = 10;
            for (int i = 0; i < n_lines; i++) {
                m = read(STDIN_FILENO, text, BUFFSIZE);
                if (m == -1) {
                    perror("read");
                }
                m = write(STDOUT_FILENO, text, m);
                if (m == -1) {
                    perror("write");
                }
            }

        } else if (n_lines == 0 && c_bytes != 0) {
            int byteCount = 0;
            while (byteCount < c_bytes) {
                m = read(STDIN_FILENO, text, BUFFSIZE);
                if (m == -1) {
                    perror("read");
                }
                byteCount += m;
                if (byteCount <= c_bytes) {
                    m = write(STDOUT_FILENO, text, m);
                    if (m == -1) {
                        perror("write");
                    }
                } else {
                    m = write(STDOUT_FILENO, text, m - (byteCount - c_bytes));
                    if (m == -1) {
                        perror("write");
                    }
                }
            }

        } else if (n_lines != 0 && c_bytes == 0) {
            for (int i = 0; i < n_lines; i++) {
                m = read(STDIN_FILENO, text, BUFFSIZE);
                if (m == -1) {
                    perror("read");
                }
                m = write(STDOUT_FILENO, text, m);
                if (m == -1) {
                    perror("write");
                }
            }

        }
    }
} // main
