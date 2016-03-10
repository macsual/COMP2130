/*
 * Author: Romario A. Maxwell <romario.maxwell@gmail.com>
 *
 * 2016
 *
 */

#include <stdlib.h>
#include <ctype.h>
#include <math.h>           /* Apparently fenv.h depends on the math library */
#include <fenv.h>
#include "spreadsheet.h"
#include "functions.h"

#pragma STDC FENV_ACCESS ON

struct fn_sym_info functions[3] = {
    {&sum, "sum"},
    {&range, "range"},
    {&avg, "average"}
};

static inline int get_cell_range_orientation(const cellref cref1,
                                                const cellref cref2);

static int sum_w_cell_count(const cellref cref1, const cellref cref2,
                                int * n_cells, real *res);

/* Compute cumulative total of cell range
 *
 * Returns the cumulative total. The input must be a horizontal or vertical
 * 1D range of cells, described by the cref2 points. The formula computes a
 * result using only values within the given range of cells; it ignores empty
 * cells and those containing text or another formula. If there are no values
 * within the given range of cells, the result defaults to zero. This is a
 * wrapper function around sum_w_cell_count.
 */
int
sum(const cellref cref1, const cellref cref2, real *res)
{
    return sum_w_cell_count(cref1, cref2, NULL, res);
}

static int
sum_w_cell_count(const cellref cref1, const cellref cref2, int * n_cells,
                    real *res)
{
    int i;
    int orientation;
    int fpe_overflow;
    int col, row;
    int range_end, range_start;
    cell *c;
    real total = 0.0f;
    real evalres;

    switch (get_cell_range_orientation(cref1, cref2)) {
        case VERTICAL:
            /* This calculation assumes that this implementation is in ASCII
             * where the values of letters are consecutive
             */
            col = tolower(cref1[0]) - (int)'a';

            /* The C standard mandates that the numbers' character
             * representations are consective/increment by 1 so this offset
             * calculation is portable
             */
            range_start = (int)cref1[1] - (int)'1';
            range_end = (int)cref2[1] - (int)'1';

            orientation = VERTICAL;

            break;

        case HORIZONTAL:
            row = (int)cref1[1] - (int)'1';

            range_start = tolower(cref1[0]) - (int)'a';
            range_end = tolower(cref2[0]) - (int)'a';

            orientation = HORIZONTAL;

            break;

        default:
            return -1;
    }

    if (n_cells != NULL)
        *n_cells = 0;

    for (i = range_start; i <= range_end; i++) {
        if (orientation == VERTICAL)
            row = i;
        else
            col = i;

        c = &spreadsheet[row][col];

        if (NUM == c->type) {
            feclearexcept(FE_OVERFLOW);

            total += c->val.f;

            fpe_overflow = fetestexcept(FE_OVERFLOW);
            if (fpe_overflow != 0)
                return -1;

            if (n_cells != NULL)
               (*n_cells)++;
        }

        if (FUNC == c->type) {
            if (eval(c->val.str, &evalres) != -1) {
                feclearexcept(FE_OVERFLOW);

                total += evalres;

                fpe_overflow = fetestexcept(FE_OVERFLOW);
                if (fpe_overflow != 0)
                    return -1;

                if (n_cells != NULL)
                   (*n_cells)++;
            }
        }
    }

    *res = total;

    return 0;
}

/* Compute arithmetic mean of range
 *
 * Returns the arithmetic mean of the numbers stored in the cells in the
 * range starting at the cell referenced by cref1 and ending at the cell
 * referenced by cref2 as a real number and returns its value as a real.
 */
int
avg(const cellref cref1, const cellref cref2, real *res)
{
    real average;
    real sum_res;
    int n_cells;
    int fpe_except;

    if (sum_w_cell_count(cref1, cref2, &n_cells, &sum_res) == -1)
        return -1;

    feclearexcept(FE_UNDERFLOW | FE_DIVBYZERO);

    average = sum_res/n_cells;

    fpe_except = fetestexcept(FE_UNDERFLOW | FE_DIVBYZERO);
    if (fpe_except != 0)
        return -1;

    *res = average;

    return 0;
}

/* Compute difference between the largest and smallest value in cell range
 *
 * Returns the difference between the largest and smallest value in the
 * input.
 */
int
range(const cellref cref1, const cellref cref2, real *res)
{
    cell *c;
    real num;
    real largest = 0.0f, smallest = 0.0f;
    real range_res;
    int i, col, row;
    int range_end, range_start;
    int fpe_underflow;
    int orientation;
    real evalres;

    switch (get_cell_range_orientation(cref1, cref2)) {
        case VERTICAL:
            /* This calculation assumes that this implementation is in ASCII
             * where the values of letters are consecutive
             */
            col = tolower(cref1[0]) - (int)'a';

            /* The C standard mandates that the numbers' character
             * representations are consective/increment by 1 so this offset
             * calculation is portable
             */
            range_start = (int)cref1[1] - (int)'1';
            range_end = (int)cref2[1] - (int)'1';

            orientation = VERTICAL;

            break;

        case HORIZONTAL:
            row = (int)cref1[1] - (int)'1';

            range_start = tolower(cref1[0]) - (int)'a';
            range_end = tolower(cref2[0]) - (int)'a';

            orientation = HORIZONTAL;

            break;

        default:
            return -1;
    }

    for (i = range_start; i <= range_end; i++) {
        if (orientation == VERTICAL)
            row = i;
        else
            col = i;

        c = &spreadsheet[col][row];

        if (NUM == c->type) {
            num = c->val.f;

            if (num < smallest)
                smallest = num;

            if (num > largest)
                largest = num;
        }

        if (FUNC == c->type) {
            if (eval(c->val.str, &evalres) != -1) {
                num = evalres;

                if (num < smallest)
                    smallest = num;

                if (num > largest)
                    largest = num;
            }
        }
    }

    feclearexcept(FE_UNDERFLOW);

    range_res = largest - smallest;

    fpe_underflow = fetestexcept(FE_UNDERFLOW);
    if (fpe_underflow != 0)
        return -1;

    *res = range_res;

    return 0;
}

/* Determine orientation of range
 *
 * The orientation of the range is determined by the endpoint and is either
 * horizontal or vertical.
 */
static inline int
get_cell_range_orientation(const cellref cref1, const cellref cref2)
{
    if ((cref2[0] == cref1[0]) &&(cref2[1] != cref1[1]))
        return VERTICAL;
    else if ((cref2[0] != cref1[0]) &&(cref2[1] == cref1[1]))
        return HORIZONTAL;
    else
        return -1;
}
