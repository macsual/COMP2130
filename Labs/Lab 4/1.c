/*
 * Author: Romario Maxwell
 * 2016
 * Course Code: COMP2130
 * Institution: The UWI, Mona
 * Lab 4
 *
 * Description: This program adds two matrices.  The user is prompted to enter
 * the values for each matrix and necessary checks are performed.
 */

#include <stdio.h>      /* BUFSIZ, NULL, stdin, fgets, puts, printf */
#include <stdlib.h>     /* EXIT_SUCCESS, EXIT_FAILURE, malloc, free */
#include <ctype.h>      /* tolower, isdigit, isspace */
#include <string.h>     /* strchr */

/* This data structure implements matrices using an array of arrays.  The
 * arrays are dynamically allocated and clients are responsible for their
 * entire lifecycle, i.e. from allocation to freeing their memory.
 */
typedef struct matrix {
    struct { int rows, cols; } dimens;
    int **arr;
} matrix;

static int parse_dimens(const char*, int*, int*);
static int parse_elements(const char*, matrix*, int);
static matrix* alloc_matrix(int, int);
static int read_elements(matrix*);
static void show_matrix(const matrix*);
static int sum_matrices(matrix*, const matrix*, const matrix*);

int
main(void)
{
    char buf[BUFSIZ];

    char *p;
    
    int rows, cols;

    matrix *A, *B, *C;

    puts("Enter the dimensions of the first matrix (e.g. 3 x 3):");

    if (fgets(buf, sizeof buf, stdin) == NULL) {
        puts("fgets failed");

        return EXIT_FAILURE;
    }

    /* strip newline character */
    p = strchr(buf, '\n');
    if (p)
        *p = '\0';

    if (parse_dimens(buf, &rows, &cols) == -1) {
        puts("parse_dimens failed");

        return EXIT_FAILURE;
    }

    A = alloc_matrix(rows, cols);
    if (NULL == A) {
        puts("alloc_matrix failed");

        return EXIT_FAILURE;
    }

    if (read_elements(A) == -1) {
        puts("read_elements failed");

        return EXIT_FAILURE;
    }

    show_matrix(A);

    puts("Enter the dimensions of the second matrix (e.g. 3 x 3):");

    if (fgets(buf, sizeof buf, stdin) == NULL) {
        puts("fgets failed");
        
        return EXIT_FAILURE;
    }

    p = strchr(buf, '\n');
    if (p)
        *p = '\0';

    if (parse_dimens(buf, &rows, &cols) == -1) {
        puts("parse_dimens failed");

        return EXIT_FAILURE;
    }

    B = alloc_matrix(rows, cols);
    if (NULL == B) {
        puts("alloc_matrix failed");

        return EXIT_FAILURE;
    }

    if (read_elements(B) == -1) {
        puts("read_elements failed");

        return EXIT_FAILURE;
    }

    show_matrix(B);

    /* rows and cols initialised from prior call */
    C = alloc_matrix(rows, cols);
    if (NULL == C) {
        puts("alloc_matrix failed");

        return EXIT_FAILURE;
    }

    if (sum_matrices(C, A, B) == -1) {
        puts("sum_matrices failed");

        return EXIT_FAILURE;
    }

    puts("The sum of the matrices is:");

    show_matrix(C);

    return EXIT_SUCCESS;
}

/* The matrix data structure implements its elements in a dynamically allocated
 * array of arrays so clients are responsible for freeing the memory.
 */
static matrix*
alloc_matrix(int rows, int cols)
{
    int i, r;

    matrix *M;

    M = malloc(sizeof(matrix));
    if (NULL == M)
        return NULL;

    M->dimens.rows = rows;
    M->dimens.cols = cols;

    /* allocate rows */
    M->arr = malloc(rows * sizeof *(M->arr));
    if (NULL == M->arr)
        return NULL;

    /* allocate columns */
    for (r = 0; r < rows; r++) {
        M->arr[r] = malloc(cols * sizeof **(M->arr));
        if (NULL == M->arr[r]) {
            for (i = 0; i < r; i++)
                free(M->arr[i]);

            return NULL;
        }
    }

    return M;
}

/* This parses the dimensions for a matrix from an input string.  Matrices have
 * two dimensions: rows and columns.  The tokens are expected to be delimited
 * by whitespace, the letter x (either case) or the string "by" (either case).
 * The function may also be used to only validate an input string if the second
 * parameter is NULL.
 */
static int
parse_dimens(const char *str, int *rows, int *cols)
{
    /* skip whitespace */
    while (isspace(*str))
        str++;

    /* token */
    if (!isdigit(*str))
        return -1;

    if (NULL != rows)
        *rows = *str - '0';

    /* advance to next character */
    str++;

    while (isspace(*str))
        str++;

    /* this means the delimitor was a space */
    if (isdigit(*str)) {
        if (NULL != cols)
            *cols = *str - '0';

        return 0;
    }
    else if ('x' != tolower(*str) &&
                ('b' != tolower(*str) && 'y' != tolower(*(str + 1))))
        return -1;

    if ('y' == tolower(*(str + 1)))
        str++;

    str++;

    while (isspace(*str))
        str++;

    if (!isdigit(*str))
        return -1;

    if (NULL != cols)
        *cols = *str - '0';

    /* TO DO: confirm that the rest of the buffer is whitespace or we are at
     * the end.
     */

    return 0;
}

/* This parses the elements to be inserted in the row (param row) of a
 * matrix (param matrix) from an input string (param str).  The number of
 * elements (tokens) is of course expected to be equal to the number of columns
 * of the matrix.  The tokens are expected to be delimited by whitespace.
 */
static int
parse_elements(const char *str, matrix *M, int row)
{
    /* TO DO: properly handle numbers with more than one digit, don't exceed
     * buffer bounds, ensure correct number of elements.
     */
    
    int col;

    /* skip whitespace */
    while (isspace(*str))
        str++;

    for (col = 0; col < M->dimens.cols; col++) {
        if (!isdigit(*str))
            return -1;

        M->arr[row][col] = *str - '0';

        str++;

        while (isspace(*str))
            str++;
    }

    return 0;
}

static int
read_elements(matrix *M)
{
    int row;

    char buf[BUFSIZ];

    char *p;

    for (row = 0; row < M->dimens.rows; row++) {
        printf("Enter elements of row %d:\n", row + 1);

        if (fgets(buf, sizeof buf, stdin) == NULL)
            return -1;

        p = strchr(buf, '\n');
        if (p)
            *p = '\0';

        if (parse_elements(buf, M, row) == -1)
            return -1;
    }

    return 0;
}

/* Add matrices A and B and store the result in matrix C.
 * The matrix data structure contains a dynamically allocated array and thus
 * clients are responsible for allocating and deallocating it.
 */
static int
sum_matrices(matrix *C, const matrix *A, const matrix *B)
{
    int r, c, rows, cols;

    if (A->dimens.rows != B->dimens.rows || A->dimens.cols != B->dimens.cols)
        return -1;

    rows = A->dimens.rows;
    cols = A->dimens.cols;

    for (r = 0; r < rows; r++) {
        for (c = 0; c < cols; c++) {
            /* TO DO: check for integer overflow */
            C->arr[r][c] = A->arr[r][c] + B->arr[r][c];
        }
    }

    return 0;
}

static void
show_matrix(const matrix *M)
{
    int row, col;

    for (row = 0; row < M->dimens.rows; row++) {
        printf("| ");
        for (col = 0; col < M->dimens.cols; col++)
            printf("%2d ", M->arr[row][col]);

        printf("|\n");
    }
}
