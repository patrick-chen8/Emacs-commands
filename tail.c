#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define BUFFSIZE 4096

/**
 * A recreation of the tail command in unix.
 */
int main(int argc, char *argv []) {

    // defining variables
    char text[BUFFSIZE];
    char tail[BUFFSIZE];
    char temp[BUFFSIZE];
    int m;
    int c_bytes = 0;
    int n_lines = 0;
    int oflags = O_RDONLY;
    int fd = 0;
    int count = 0;
    int newlines =  0;
    int total = 0;
    int arg;


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
            newlines = 0;
            total = 0;
            count = 0;

            // printing banner
            if (fd > 0) {
                printf("\n==> standard input <==\n");
            } else if (fd == 0 && argc >= 5) {
                printf("==> standard input <==\n");
            }
            fd = -1;

            // consdering all options with c and n
            if (n_lines == 0 && c_bytes == 0) {
                n_lines = 10;

                // reading in all the input to text array and filling temp array
                while ((m = read(STDIN_FILENO, text, BUFFSIZE)) > 0) {
                    if (m == -1) {
                        perror("read");
                    }
                    for (int i = 0; i < m; i++) {
                        temp[count] = text[i];
                        count++;
                    }
                    total += m;
                }

                // using temp array to count total newlines
                for (int i = 0; i < total; i++) {
                    if (temp[i] == '\n') {
                        newlines++;
                    }
                }

                // solving for the index where writing will start
                int lineDiff = newlines - n_lines;
                if (lineDiff == 0) {
                    m = write(STDOUT_FILENO, temp, total);
                    if (m == -1) {
                        perror("write");
                    }
                } else {
                    int lineCounter = 0;
                    for (int i = 0; i < total; i++) {
                        if (temp[i] == '\n') {
                            lineCounter++;
                            if (lineCounter == lineDiff) {
                                count = i + 1;
                                break;
                            }
                        }
                    }

                    // filling tail array with correct values
                    int size = total - count;
                    for (int i = 0; i < size; i++) {
                        tail[i] = temp[count];
                        count++;
                    }

                    m = write(STDOUT_FILENO, tail, size);
                    if (m == -1) {
                        perror("write");
                    }
                }

            } else if (n_lines == 0 && c_bytes != 0) {
                total = 0;
                while ((m = read(STDIN_FILENO, text, BUFFSIZE)) > 0) {
                    if (m == -1) {
                        perror("read");
                    }
                    total += m;
                }
                if (total <= c_bytes) {
                    m = write(STDOUT_FILENO, text, m);
                    if (m == -1) {
                        perror("write");
                    }
                } else {
                    count = total - c_bytes;
                    for (int i = 0; i < c_bytes; i++) {
                        tail[i] = text[count];
                        count++;
                    }
                    m = write(STDOUT_FILENO, tail, c_bytes);
                    if (m == -1) {
                        perror("write");
                    }
                }

            } else if (n_lines != 0 && c_bytes == 0) {
                while ((m = read(STDIN_FILENO, text, BUFFSIZE)) > 0) {
                    if (m == -1) {
                        perror("read");
                    }
                    for (int i = 0; i < m; i++) {
                        temp[count] = text[i];
                        count++;
                    }
                    total += m;
                }


                for (int i = 0; i < total; i++) {
                    if (temp[i] == '\n') {
                        newlines++;
                    }
                }

                int lineDiff = newlines - n_lines;
                if (lineDiff == 0) {
                    m = write(STDOUT_FILENO, temp, total);
                    if (m == -1) {
                        perror("write");
                    }
                } else {
                    int lineCounter = 0;
                    for (int i = 0; i < total; i++) {
                        if (temp[i] == '\n') {
                            lineCounter++;
                            if (lineCounter == lineDiff) {
                                count = i + 1;
                                break;
                            }
                        }
                    }

                    int size = total - count;
                    for (int i = 0; i < size; i++) {
                        tail[i] = temp[count];
                        count++;
                    }

                    m = write(STDOUT_FILENO, tail, size);
                    if (m == -1) {
                        perror("write");
                    }
                }
            }
            // reading files
        } else if ((fd = open(argv[arg], oflags)) > 0) {

            // printing banner
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

            if (n_lines == 0 && c_bytes == 0) {
                n_lines = 10;
                m = read(fd, text, BUFFSIZE);
                if (m == -1) {
                    perror("read");
                }

                for (int i = m; i > 0; i--) {
                    if (text[i] == '\n') {
                        newlines++;
                    }
                }

                int lineDiff = newlines - n_lines;
                int lineCounter = 0;
                for (int i = 0; i < m; i++) {
                    if (text[i] == '\n') {
                        lineCounter++;
                        if (lineCounter == lineDiff) {
                            count = i + 1;
                            break;
                        }
                    }
                }

                int size = m - count;
                for (int i = 0; i < size; i++) {
                    tail[i] = text[count];
                    count++;
                }

                m = write(STDOUT_FILENO, tail, size);
                if (m == -1) {
                    perror("write");
                }

            } else if (n_lines != 0 && c_bytes == 0) {
                m = read(fd, text, BUFFSIZE);
                if (m == -1) {
                    perror("read");
                }

                for (int i = m; i > 0; i--) {
                    if (text[i] == '\n') {
                        newlines++;
                    }
                }

                int lineDiff = newlines - n_lines;
                int lineCounter = 0;
                for (int i = 0; i < m; i++) {
                    if (text[i] == '\n') {
                        lineCounter++;
                        if (lineCounter == lineDiff) {
                            count = i + 1;
                            break;
                        }
                    }
                }

                int size = m - count;
                for (int i = 0; i < size; i++) {
                    tail[i] = text[count];
                    count++;
                }

                m = write(STDOUT_FILENO, tail, size);
                if (m == -1) {
                    perror("write");
                }

            } else if (n_lines == 0 && c_bytes != 0) {
                m = read(fd, text, BUFFSIZE);
                if (m == -1) {
                    perror("read");
                }

                int byteDiff = m - c_bytes;
                for (int i = 0; i < c_bytes; i++) {
                    tail[i] = text[byteDiff];
                    byteDiff++;
                }
                m = write(STDOUT_FILENO, tail, c_bytes);
                if (m == -1) {
                    perror("write");
                }
            }
        }
    }

    // if there are no files in the command line argument
    if (argc == 1 || ((c_bytes != 0 || n_lines !=0) && fd == 0)) {
        newlines = 0;
        total = 0;
        count = 0;

        if (n_lines == 0 && c_bytes == 0) {
            n_lines = 10;
            while ((m = read(STDIN_FILENO, text, BUFFSIZE)) > 0) {
                if (m == -1) {
                    perror("read");
                }
                for (int i = 0; i < m; i++) {
                    temp[count] = text[i];
                    count++;
                }
                total += m;
            }


            for (int i = 0; i < total; i++) {
                if (temp[i] == '\n') {
                    newlines++;
                }
            }

            int lineDiff = newlines - n_lines;
            if (lineDiff == 0) {
                m = write(STDOUT_FILENO, temp, total);
                if (m == -1) {
                    perror("write");
                }
            } else {
                int lineCounter = 0;
                for (int i = 0; i < total; i++) {
                    if (temp[i] == '\n') {
                        lineCounter++;
                        if (lineCounter == lineDiff) {
                            count = i + 1;
                            break;
                        }
                    }
                }

                int size = total - count;
                for (int i = 0; i < size; i++) {
                    tail[i] = temp[count];
                    count++;
                }

                m = write(STDOUT_FILENO, tail, size);
                if (m == -1) {
                    perror("write");
                }
            }


        } else if (n_lines == 0 && c_bytes != 0) {
            total = 0;
            while ((m = read(STDIN_FILENO, text, BUFFSIZE)) > 0) {
                if (m == -1) {
                    perror("read");
                }
                total += m;
            }
            if (total <= c_bytes) {
                m = write(STDOUT_FILENO, text, m);
                if (m == -1) {
                    perror("write");
                }
            } else {
                count = total - c_bytes;
                for (int i = 0; i < c_bytes; i++) {
                    tail[i] = text[count];
                    count++;
                }
                m = write(STDOUT_FILENO, tail, c_bytes);
                if (m == -1) {
                    perror("write");
                }
            }

        } else if (n_lines != 0 && c_bytes == 0) {
            while ((m = read(STDIN_FILENO, text, BUFFSIZE)) > 0) {
                if (m == -1) {
                    perror("read");
                }
                for (int i = 0; i < m; i++) {
                    temp[count] = text[i];
                    count++;
                }
                total += m;
            }


            for (int i = 0; i < total; i++) {
                if (temp[i] == '\n') {
                    newlines++;
                }
            }

            int lineDiff = newlines - n_lines;
            if (lineDiff == 0) {
                m = write(STDOUT_FILENO, temp, total);
                if (m == -1) {
                    perror("write");
                }
            } else {
                int lineCounter = 0;
                for (int i = 0; i < total; i++) {
                    if (temp[i] == '\n') {
                        lineCounter++;
                        if (lineCounter == lineDiff) {
                            count = i + 1;
                            break;
                        }
                    }
                }

                int size = total - count;
                for (int i = 0; i < size; i++) {
                    tail[i] = temp[count];
                    count++;
                }

                m = write(STDOUT_FILENO, tail, size);
                if (m == -1) {
                    perror("write");
                }
            }
        }
    }
} // main
