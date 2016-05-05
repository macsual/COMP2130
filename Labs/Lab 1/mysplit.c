/*
 * Author: Romario Maxwell
 * 2016
 * Course Code: COMP2130
 * Institution: The UWI, Mona
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> /* isupper() */

int
main(void)
{
    int i, j;

    char name[50], first[25], last[25];

    printf("What is your name? ");

    scanf("%49s", name);

    /* finding the index of the first character of the first name */
    for (i = 0; i < sizeof(name); i++) {
        if (isupper(name[i]))
            break;
    }

    /* Storing the first name.
     * The loop ends once the first character of the last name is encountered
     * or when the maximum amount of characters that
     * can be stored in first are written to it
     */
    for (j = 0; j < sizeof(first); j++) {
        first[j] = name[i];

        i++;

        /* the start of the last name */
        if (isupper(name[i])) {
            if (j < sizeof(first))
                j++;

            break;
        }
    }
    first[j] = '\0';

    /* finding the index of the first character of the last name */
    for (i = strlen(first); i < sizeof(name); i++) {
        if (isupper(name[i]))
            break;
    }

    /* Storing the last name.
     * The loop ends once the end of the name string is encountered
     * or when the maximum amount of characters that can be stored in last
     * are written to it
     */
    for (j = 0; j < sizeof(last); j++) {
        last[j] = name[i];

        i++;

        if (name[i] == '\0') {
            if (j < sizeof(last))
                j++;

            break;
        }
    }
    last[j] = '\0';

    printf("Name: %s\n", name);
    printf("First name: %s\n", first);
    printf("Last name: %s\n", last);

    return EXIT_SUCCESS;
}
