/*
 * Author: Romario A. Maxwell <romario.maxwell@gmail.com>
 *
 * 2016
 *
 * Description:
 *     Provides a simple spreadsheet. The program allows the user to enter
 * numeric values, text, or formulas into any cell. As new data is entered, the
 * spreadsheet is updated and redisplayed. The program runs until the user
 * quits.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spreadsheet.h"

int
main(void)
{
    char buf[BUFSIZ];
    char *p;
    cellref cref;
    cell *c;

    for (;;) {
        show_spreadsheet();

        puts("Enter cell");

        if (fgets(buf, sizeof buf, stdin) == NULL) {
            fprintf(stderr, "fgets failed");

            /* if fgets fails, the content of the array being written to is
             * indeterminate(because of undefined behavior). It is necessary
             * to reset the string to a known value to avoid errors on
             * subsequent string manipulation functions.
            */
            *buf = '\0';

            continue;
        }

        /* strip newline character */
        p = strchr(buf, '\n');
        if (p)
            *p = '\0';

        if (!iscellref(buf)) {
            fprintf(stderr, "Invalid cell reference %s\n", buf);

            continue;
        }

        snprintf(cref, 3, "%s", buf);

        c = cell_addr(cref);
        if (NULL == c) {
            fprintf(stderr, "cell_addr failed %s\n", cref);

            continue;
        }

        if (FUNC == c->type)
            printf("Current formula in cell: %s\n", c->val.str);

        putchar((int)'\n');

        puts("Enter cell data");

        if (fgets(buf, sizeof buf, stdin) == NULL) {
            fprintf(stderr, "fgets failed");

            /* if fgets fails, the content of the array being written to is
             * indeterminate(because of undefined behavior). It is necessary
             * to reset the string to a known value to avoid errors on
             * subsequent string manipulation functions.
            */
            *buf = '\0';

            continue;
        }

        p = strchr(buf, '\n');
        if (p)
            *p = '\0';

        c = cell_addr(cref);
        if (NULL == c) {
            fprintf(stderr, "cell_addr failed %s\n", cref);

            continue;
        }

        if (strtocellval(buf, c) == -1)
            fprintf(stderr, "Invalid cell value %s\n", buf);

    }

    return EXIT_SUCCESS;
}
