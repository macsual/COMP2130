/*
 * Author: Romario Maxwell
 * 2016
 * Course Code: COMP2130
 * Institution: The UWI, Mona
 */

#include <stdio.h>
#include <stdlib.h>     /* strtoull, EXIT_SUCESS, EXIT_FAILURE */
#include <string.h>     /* strlen */
#include <limits.h>     /* ULLONG_MAX */
#include <errno.h>

#define NUMDIGITS 10

static inline int num_digits(unsigned long long);
static inline int sum_digits(unsigned long long);

int
main(void)
{
    /* As per the constraint:
     * "The program should work for any number having one to ten digits."
     * type unsigned long long int is implementation dependent but it might
     * be able to hold 10 digits.
     */
    unsigned long long num;

    /* As per the constraint:
     * "The program should work for any number having one to ten digits."
     */
    char buff[NUMDIGITS + 1];

    char *end_ptr;

    puts("Enter a positive integer");

    if (fgets(buff, sizeof(buff), stdin) == NULL) {
        if (puts("EOF or read error\n") == EOF) {
        /* Handle error */
        }

        return EXIT_FAILURE;
    } else {
        errno = 0;

        if (*buff == '-') {
            /* fgets includes a newline character in the input if the input
             * is shorter than the size of the input buffer
             */
            buff[strlen(buff) - 1] = '\0';

            printf("%s is not a positive integer\n", buff);

            return EXIT_FAILURE;
        }

        num = strtoull(buff, &end_ptr, 10);

        if (end_ptr == buff) {
            if (puts("not valid numeric input\n") == EOF) {
              /* Handle error */
            }

            return EXIT_FAILURE;
        }
        else if (num == ULLONG_MAX && errno == ERANGE) {
            if (puts("number out of range\n") == EOF) {
              /* Handle error */
            }

            return EXIT_FAILURE;
        }
        /* fgets includes a newline character in the input if the input is 
         * shorter than the size of the input buffer
         */
        else if (*end_ptr != '\n' && *end_ptr != '\0') {
            if (puts("extra characters on input line\n") == EOF) {
              /* Handle error */
            }

            return EXIT_FAILURE;
        }
    }

    printf("The sum of the digits in %llu is %d.\n", num, sum_digits(num));

    return EXIT_SUCCESS;
}

static inline int
sum_digits(unsigned long long num)
{
    if (num < 10)
        return num;
    else
        return num%10 + sum_digits(num/10);
}
