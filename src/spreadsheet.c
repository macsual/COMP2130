/*
 * Author: Romario A. Maxwell <romario.maxwell@gmail.com>
 *
 * 2016
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>           /* Apparently fenv.h depends on the math library */
#include <fenv.h>
#include "spreadsheet.h"
#include "functions.h"

#pragma STDC FENV_ACCESS ON

cell spreadsheet[9][9];

/* Get cell's memory address
 *
 * Returns the cell's memory address referenced by cref.
 *
 * The cell's memory address is returned on success and a null pointer is
 * returned otherwise.
 */
void*
cell_addr(const cellref cref)
{
    int row, col;

    if (!iscellref(cref))
        return NULL;

    row = (int)cref[1] - (int)'1';
    col = tolower(cref[0]) - (int)'a';

    return &spreadsheet[row][col];
}

/* Evaluate function call
 *
 * Parses the C string str interpreting its content as a function call with its
 * arguments and calls the function and stores the return value as a long
 * double in the memory address pointed by maddr.
 *
 * 0 is returned on success and -1 otherwise.
 */
int
eval(const char *str, real *maddr)
{
    int i;
    struct fn_sym_info *sym;
    char fn_ident[IDENT_LEN + 1];
    char fn_arg_ident[IDENT_LEN + 1];
    char *p;
    cellref crefs[2];
    real res;
    const char L_PAREN = '(';
    const char R_PAREN = ')';
    const char ARG_DELIM = ',';

    while (isspace(*str)) str++;

    if ('\0' == *str) /* end of string therefore string is empty */
        return -1;

    /* the first character of an identifier can not be a number literal */
    if (isdigit(*str))
        return -1;

    /* store function identifier from str */
    for (i = 0; i < sizeof fn_ident - 1; i++) {
        if ('\0' == str[i] || isspace(str[i]) || L_PAREN == str[i]) {
            i--;
            break;
        }

        fn_ident[i] = tolower(str[i]);
    }
    fn_ident[i + 1] = '\0';

    /* lookup symbol information for user provided function */
    for (i = 0; i < sizeof functions / sizeof functions[0]; i++) {
        if (strcmp(fn_ident, functions[i].ident) == 0) {
            sym = &functions[i];

            break;
        }
    }

    /* undefined function */
    if (sizeof functions / sizeof functions[0] == i)
        return -1;

    /* args list opening */
    p = strchr(str, L_PAREN);
    if (!p)
        return -1;

    i = 0;
    do {

        if (i >= 2) /* too many args */
            return -1;

        p++; /* advance to next character in sequence */

        while (isspace(*p)) p++;

        if (!isalpha(*p))
            return -1;
        else
            fn_arg_ident[0] = *p;

        p++;

        if (!isdigit(*p))
            return -1;
        else
            fn_arg_ident[1] = *p;

        fn_arg_ident[2] = '\0';

        snprintf(crefs[i], 3, "%s", fn_arg_ident);

        if (!iscellref(crefs[i]))
            return -1;

        p++;

        while (isspace(*p)) p++;

        i++;
    } while (ARG_DELIM == *p);

    /* args list closing */
    if (R_PAREN != *p)
        return -1;

    if (sym->addr(crefs[0], crefs[1], &res) == -1)
        return -1;

    *maddr = res;

    return 0;
}

/* Convert string to real
 *
 * Parses the C string str interpreting its content as a floating point number
 * (according to a Jamaican locale) and stores its value as a real in
 * the memory address pointed by num. str is allowed to be NULL in which case
 * the return value may be used to determine the validity of str.
 *
 * 0 is returned on success and -1 on failure and errno may be set to ERANGE by
 * an internal call to strtold.
 */
int
strtoreal(const char *str, real *num)
{
    char *endptr;
    real res;
    int fpe_underflow;

    errno = 0;

    feclearexcept(FE_UNDERFLOW);

    res = strtold(str, &endptr);

    fpe_underflow = fetestexcept(FE_UNDERFLOW);
    if (fpe_underflow != 0)
        return -1;

    /* couldn't convert to a valid number */
    if (NULL == endptr)
        return -1;
    /* number couldn't be represented in the range */
    else if ((HUGE_VALL == *num || -HUGE_VALL == *num) && ERANGE == errno)
        return -1;
    /* extra characters after number */
    else if (*endptr !=  '\0')
        return -1;

    if (num != NULL)
        *num = res;

    return 0;
}

/* Convert string to cell data
 *
 * Parses the C string str interpreting it as a cell data type and stores its
 * value at the memory address pointed by c.
 */
int
strtocellval(const char *str, cell *c)
{
    real num;

    while (isspace(*str)) str++;

    /* number */
    if ('+' ==  *str || '-' == *str ||('.' == *str && isdigit(str + 1))
                || isdigit(*str)) {
        if (strtoreal(str, &num) == -1)
            return -1;

        c->type = NUM;
        c->val.f = num;
    }
    /* formula */
    else if ('=' == *str) {
        c->type = FUNC;
        snprintf(c->val.str, 256, "%s", str + 1);
    }
    /* text */
    else {
        c->type = TEXT;
        snprintf(c->val.str, 256, "%s", str);
    }

    return 0;
}

/* Validate cell reference
 *
 * Parses cell reference cref interpreting its content as the reference to
 * a cell in the spreadsheet according to the information that follows. The
 * spreadsheet is 9 x 9 cells. Rows are labeled with numbers, starting
 * from 1, and columns are labeled with letters, starting from A. Cells are
 * referenced by a letter number pair, with no space or symbol between; for
 * example, E9. Lowercase and uppercase letters to reference cells are allowed
 * and recognised.
 */
int
iscellref(const cellref cref)
{
    int row, col;

    if (!isalpha(cref[0]) || !isdigit(cref[1]))
        return 0;

    row = ((int)cref[1] -(int)'1') + 1;
    col = (tolower(cref[0]) -(int)'a') + 1;

    if (col < 1 || col > 9)
        return 0;

    if (row < 1 || row > 9)
        return 0;

    return 1;
}

void
show_spreadsheet(void)
{
    int row, col;
    cell *c;
    real res;

    putchar((int)'\n');

    printf("%6c %12c %12c %12c %12c %12c %12c %12c %12c\n",
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I');

    for (row = 0; row < 9; row++) {
        printf("%d ", row + 1);
        
        for (col = 0; col < 9; col++) {
            c = &spreadsheet[row][col];

            switch (c->type) {
                case NUM:
                    printf("%12.12Lg ", c->val.f);
                    break;

                case FUNC:
                    if (eval(c->val.str, &res) == -1)
                        printf("%-12.12s ", "ERR");
                    else
                        printf("%12.12Lg ", res);
                    break;

                case TEXT:
                    printf("%-12.12s ", c->val.str);
                    break;

                default:
                    printf("%-12.12s ", "N/A");
                    break;
            }
        }

        putchar((int)'\n');
        putchar((int)'\n');
    }

    putchar((int)'\n');
}
