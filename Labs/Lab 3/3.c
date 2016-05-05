/*
 * Author: Romario Maxwell
 * 2016
 * Course Code: COMP2130
 * Institution: The UWI, Mona
 * Lab 3
 */

#include <stdio.h>      /* NULL, FILE, snprintf, fopen, printf, feof, fscanf */
#include <stdlib.h>     /* malloc, EXIT_SUCCESS, EXIT_FAILURE */
#include <string.h>     /* strlen */

#define WORDLEN_MIN 3
#define WORDLEN_MAX 15

const char *prog_name;

static void show_usage(void);

int
main(int argc, char *argv[])
{
    int i;

    /* each element stores the count of words of a specific length */
    int counts[WORDLEN_MAX - WORDLEN_MIN + 1] =
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    /* number of bytes in a word */
    int length;

    /* stream for user provided file */
    FILE *fp;

    /* stores the path of the user provided file */
    char *filename;

    /* As per the constraint:
     * "The program should report the total word counts of all lengths
     * between 3 and 15 bytes. Words with lengths outside that range should not
     * be counted."
     * The first + 1 is so that we can detect if a word is greater than
     * 15 bytes and the last + 1 is for the terminating null character
     */
    char word[WORDLEN_MAX + 1 + 1];

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
        puts("malloc failed");

        return EXIT_FAILURE;
    }

    snprintf(filename, strlen(argv[1]) + 1, "%s", argv[1]);

    fp = fopen(filename, "r");
    if (NULL == fp) {
        puts("fopen failed");

        return EXIT_FAILURE;
    }

    while (!feof(fp)) {
        fscanf(fp, "%16s", word);

        length = strlen(word);

        if (length < WORDLEN_MIN || length > WORDLEN_MAX)
            continue;
        else
            counts[length - WORDLEN_MIN]++;
    }

    puts("Total word counts of all lengths between 3 and 15 bytes:");
    for (i = 0; i < WORDLEN_MAX - WORDLEN_MIN + 1; i++)
        printf("Length %d: %d\n", i + WORDLEN_MIN, counts[i]);

    return EXIT_SUCCESS;
}

static void
show_usage(void)
{
    printf("Usage:\n"
            "\t%s filename\n\n", prog_name);
}
