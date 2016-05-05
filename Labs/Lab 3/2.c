/*
 * Author: Romario Maxwell
 * 2016
 * Course Code: COMP2130
 * Institution: The UWI, Mona
 * Lab 3
 */

#include <stdio.h>      /* EOF, size_t */

/* glibc or something else has a function called log defined already that is
 * interfering with the linking process
 */
#define log cs2130_log

int write_inventory(const char *);

/* Assumption: log is a global variable because it is declared as part of the
 * struct definition
 */
struct inventory
{
    char name[30];
    int count;
    float price;
} log[75];

int
write_inventory(const char * filename)
{
    FILE *fp;

    int i;

    /* Assumption: there is no exisiting file/okay for existing file to be
     * overwritten
     */
    fp = fopen(filename, "w");
    if (NULL == fp)
        return -1;
    else {
        /* note that as the array log is a global variable it is automatically
         * defined/zero initialised
         */
        for (i = 0; i < (sizeof log / sizeof log[0]); i++)
            fprintf(fp, "%s %d %.2f\n",
                            log[i].name, log[i].count, log[i].price);

        if (fclose(fp) == EOF)
            return -1;

        return 0;
    }
}
