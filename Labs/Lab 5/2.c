/*
 * Author: Romario Maxwell
 * 2016
 * Course Code: COMP2130
 * Institution: The UWI, Mona
 * Lab 5
 */

#include <stdio.h>      /* NULL, FILE, fopen, fprintf */
#include <stdlib.h>     /* EXIT_SUCCESS, EXIT_FAILURE */
#include <unistd.h>     /* fork, getpid */

int
main(void)
{
    FILE *fp;

    fp = fopen("myfile.txt", "w");
    if (NULL == fp)
        return EXIT_FAILURE;

    switch (fork()) {
        /*  error in parent  */
        case -1:
            fprintf(stderr, "fork failed\n");
            return EXIT_FAILURE;

        /*  in child  */
        case 0:
            fprintf(fp,
                "I am a child\n"
                "I am writing in this file\n"
                "My process id is %d\n", getpid());

            return EXIT_SUCCESS;

        /*  in parent, pid of child  */
        default:
            return EXIT_SUCCESS;
    }
}
