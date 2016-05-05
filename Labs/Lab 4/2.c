/*
 * Author: Romario Maxwell
 * 2016
 * Course Code: COMP2130
 * Institution: The UWI, Mona
 * Lab 4
 *
 * Description: This function takes a decimal number and converts it to binary.
 */

#include <stddef.h>     /* size_t */
#include <string.h>     /* strlen */

/* The instructions did not specify what representation to use for negative
 * decimals so this implementation uses sign and magnitude representation */
int decimal_to_binary(int, size_t, char*);

/* num is the decimal number to be converted to binary, n is the size of
 * parameter s, the null terminated C string which is used to store
 * the binary representation
 */
int
decimal_to_binary(int num, size_t n, char *s)
{
    int i, n_bits;

    unsigned num_u, bit;

    /* the number of bits required to represent the unsigned num in memory that
     * has the same magnitude as parameter n.
     */
    n_bits = 8 * sizeof num;

    /* this check ensures that string s is large enough to store num's binary
     * representation.
     * 1 is subtracted from n to discard the terminating null character from
     * this calculation and 1 is added to n_bits to account for the sign
      * character to be used in the binary represention.
     */
    if (n - 1 < 1 + n_bits)
        return -1;

    /* bitwise operations should only take unsigned operands because signed
     * ones are implementation defined.
     */
    num_u = num < 0 ? -num : num;

    /* the most significant bit is set to 1 for negative numbers and 0 for
     * non-negative numbers.
     */
    s[0] = num < 0 ? '1' : '0';

    for (bit = 1u << n_bits - 1, i = 1; bit != 0u; bit >>= 1u, i++)
        s[i] = (num_u & bit) ? '1' : '0';

    s[i] = '\0';

    return 0;
}
