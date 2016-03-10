/*
 * Author: Romario A. Maxwell <romario.maxwell@gmail.com>
 *
 * 2016
 *
 */

#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#define IDENT_LEN 31

enum range_orientation {VERTICAL, HORIZONTAL};
enum cell_type {TEXT = 1, NUM, FUNC};

typedef char cellref[3];
typedef long double real;

/* cell in spreadsheet */
typedef struct cell {
    unsigned int type;
    union { long double f; char str[256]; } val;
} cell;

extern cell spreadsheet[9][9];

extern int strtoreal(const char *str, real *num);
extern int strtocellval(const char *str, cell *c);
extern int eval(const char *str, real *maddr);
extern void show_spreadsheet(void);
extern int iscellref(const cellref cref);
extern void* cell_addr(const cellref cref);

#endif /* SPREADSHEET_H */
