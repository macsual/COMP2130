/*
 * Author: Romario Maxwell
 * 2016
 * Course Code: COMP2130
 * Institution: The UWI, Mona
 */

#include <stdio.h>
#include <stdlib.h>     /* EXIT_SUCCESS, EXIT_FAILURE */
#include <errno.h>
#include <limits.h>     /* INT_MIN, INT_MAX, LONG_MIN & LONG_MAX */

static void show_usage(void);
static void show_args(int, char **);
static int parse_int(const char *, int *);

int
main(int argc, char *argv[])
{
    int dividend, divisor;
    double quotient;
    int si;
    int ret;

    if (argc != 3) {
        show_usage();

        show_args(argc, argv);

        return EXIT_FAILURE;
    }

    ret = parse_int(argv[1], &si);
    if (ret == -1) {
        show_args(argc, argv);

        return EXIT_FAILURE;
    }
    dividend = si;

    ret = parse_int(argv[2], &si);
    if (ret == -1) {
        show_args(argc, argv);

        return EXIT_FAILURE;
    }
    divisor = si;

    if (dividend <= divisor) {
        fprintf(stderr, "The first argument is smaller than "
                        "the second argument.\n");

        show_args(argc, argv);

        return EXIT_FAILURE;
    }

    if (divisor == 0) {
        fprintf(stderr, "Cannot perform division by zero.\n");

        show_args(argc, argv);

        return EXIT_FAILURE;
    }

    quotient = (double)dividend/divisor;

    printf("The quotient of dividend %d and divisor %d is %.3f "
            "within a precision of three decimal places.\n",
            dividend, divisor, quotient);

    show_args(argc, argv);

    return EXIT_SUCCESS;
}

static void
show_args(int argc, char *argv[])
{
    int i;

    printf("%d command line argument%s:\n", argc, argc == 1 ? "" : "s");
    for (i = 0; i < argc; i++)
        printf("\tArgument %d: %s\n", i + 1, argv[i]);
}

static int
parse_int(const char *s, int *si)
{
    long sl;
    char *end_ptr;

    errno = 0;

    sl = strtol(s, &end_ptr, 10);

    if (end_ptr == s) {
        fprintf(stderr, "%s: not a decimal number\n", s);

        return -1;
    }
    else if (*end_ptr != '\0') {
        fprintf(stderr, "%s: extra characters at end of input: %s\n", s, end_ptr);

        return -1;
    }
    else if ((LONG_MIN == sl || LONG_MAX == sl) && ERANGE == errno) {
        fprintf(stderr, "%s out of range of type long\n", s);

        return -1;
    }
    else if (sl > INT_MAX) {
        fprintf(stderr, "%ld greater than INT_MAX\n", sl);

        return -1;
    }
    else if (sl < INT_MIN) {
        fprintf(stderr, "%ld less than INT_MIN\n", sl);

        return -1;
    }
    else
        *si = (int)sl;

    return 0;
}

static void
show_usage(void)
{
    printf("Usage:\n"
            "\tmydivide int1 int2\n\n");
}
