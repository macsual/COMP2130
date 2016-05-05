/*
 * Author: Romario Maxwell
 * 2016
 * Course Code: COMP2130
 * Institution: The UWI, Mona
 * Lab 3
 */

#include <stdio.h>          /* NULL, BUFSIZ, size_t, fgets, puts, printf */
#include <stdlib.h>         /* EXIT_SUCCESS, EXIT_FAILURE */
#include <string.h>         /* strchr, strlen */
#include <errno.h>          /* errno, EINTR */

#include <unistd.h>         /* open, write, close */
#include <fcntl.h>          /* O_WRONLY, O_NONBLOCK, O_NOCTTY */

/* Not technically required, but needed on some UNIX distributions */

#include <sys/types.h>      /* ssize_t */
#include <sys/stat.h>

/* I need to properly handle possible truncation of the user input in the
 * buffer and report when an operation fails because of inadequate
 * permissions (particularly because virtual terminals have permissions
 * rw--w---- and are owned by root and the root group).  Additionally, I think
 * my error checking may be improved with regards to interruption from signals
 */
int
main(void)
{
    int tty_fd;

    char buf[BUFSIZ];

    char *p;

    ssize_t bytes;

    size_t txt_len, bytes_written;

    for (;;) {
        puts("Enter the path of device file of the terminal on which to "
                "display text");

        fgets(buf, sizeof(buf), stdin);
        if (NULL == buf) {
            puts("fgets failed");

            /* If fgets fails, the content of the array being written to is
             * indeterminate (because of undefined behavior).  It is necessary
             * to reset the string to a known value to avoid errors on
             * subsequent string manipulation functions.
            */
            *buf = '\0';

            continue;
        }

        /* strip trailing newline character */

        p = strchr(buf, '\n');
        if (p)
            *p = '\0';

        tty_fd = open(buf, O_WRONLY | O_NOCTTY);
        if (-1 == tty_fd && errno != EINTR) {
            puts("open failed");

            continue;
        }

        if (!isatty(tty_fd) && (EINVAL == errno || ENOTTY == errno)) {
            printf("Path %s is not a terminal\n", buf);

            if (close(tty_fd) == -1 && errno != EINTR) {
                puts("close failed");

                return EXIT_FAILURE;
            }

            continue;
        }

        printf("Enter text to display in terminal %s\n", buf);

        fgets(buf, sizeof buf, stdin);
        if (NULL == buf) {
            puts("fgets failed");

            *buf = '\0';

            if (close(tty_fd) == -1 && errno != EINTR) {
                puts("close failed");

                return EXIT_FAILURE;
            }

            continue;
        }

        /* this section on write needs some more work */

        txt_len = strlen(buf);
        bytes_written = (size_t)0;
        while (bytes_written < txt_len) {
            bytes = write(tty_fd, buf, txt_len);
            if ((ssize_t)-1 == bytes && errno != EINTR) {
                puts("write failed");

                break;
            }

            bytes_written += bytes;
        }

        if (close(tty_fd) == -1 && errno != EINTR) {
            puts("close failed");

            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
