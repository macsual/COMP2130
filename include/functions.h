/*
 * Author: Romario A. Maxwell <romario.maxwell@gmail.com>
 *
 * 2016
 *
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "spreadsheet.h"

/* function symbol information */
struct fn_sym_info { int(*addr)(); char ident[IDENT_LEN + 1]; };

extern struct fn_sym_info functions[3];

extern int sum(const cellref cref1, const cellref cref2, real *res);
extern int range(const cellref cref1, const cellref cref2, real *res);
extern int avg(const cellref cref1, const cellref cref2, real *res);

#endif /* FUNCTIONS_H */
