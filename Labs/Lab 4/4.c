/*
 * Author: Romario Maxwell
 * 2016
 * Course Code: COMP2130
 * Institution: The UWI, Mona
 * Lab 4
 *
 * Description: This program accepts a filename for a C file as a command line
 * argument and checks the C code if there are open comments
 * (obviously multiline) that were not closed.  The program terminates
 * immediately when it finds this situation and reports the error.
 */

#include <stdio.h>  /* NULL, FILE, EOF, fgetc, feof, ferror, printf, fprintf */
#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE, malloc */
#include <string.h> /* strlen */

const char *prog_name;

static void show_usage(void);

/* TODO: account for comments beginning in a single line comment and in string
 * literals or any other areas where they are invalid */
int
main(int argc, char *argv[])
{
    int c;

    /* flag to indicate the prescence of its namesake. */
    int unclosed_comment = 0;

    /* stream for user provided file */
    FILE *fp;
    
    /* stores the path of the user provided file */
    char *filename;

    if (argc && argv[0])
        prog_name = argv[0];
    else
        prog_name = "";

    if (argc != 2) {
        show_usage();
        
        return EXIT_FAILURE;
    }

    filename = malloc(strlen(argv[1]) + 1);
    if (NULL == filename) {
        fprintf(stderr, "malloc failed\n");

        return EXIT_FAILURE;
    }
    
    (void)snprintf(filename, strlen(argv[1]) + 1, "%s", argv[1]);

    fp = fopen(filename, "r");
    if (NULL == fp) {
        fprintf(stderr, "fopen failed\n");

        return EXIT_FAILURE;
    }

    do {
        c = fgetc(fp);

        /* search for beginning of comment */
        if ('/' == (char)c) { /* potentially the beginning of a comment */
            /* advance to next character in stream */
            c = fgetc(fp);

            if ('*' == (char)c)
                unclosed_comment = 1;
            /* the asterisk must immediately follow the forward slash */
            else
                /* continue searching for comments */
                continue;

            c = fgetc(fp);

            /* search for end of comment */
            while ((char)c != EOF) {
                c = fgetc(fp);

                /* potentially the beginning of comment closing */
                if ('*' == (char)c) {
                    c = fgetc(fp);
                    
                    /* the forward slash must immediately follow the asterisk */
                    if ('/' == (char)c) {
                        unclosed_comment = 0;

                        /* continue searching for comments */
                        break;
                    }
                }
            }
        }
    } while (c != EOF);

    if (feof(fp)) { /* end of file */
        if (unclosed_comment) {
            fprintf(stderr, "There is an unclosed comment\n");

            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    } else if (ferror(fp)) { /* file error */
        fprintf(stderr, "An error occurred while reading the file\n");

        return EXIT_FAILURE;
    } else { /* received a character that resembles EOF */
        fprintf(stderr, "A character that resembles EOF was encountered\n");

        return EXIT_FAILURE;
    }
}

static void
show_usage(void)
{
    printf("Usage:\n"
            "\t%s filename\n\n", prog_name);
}
