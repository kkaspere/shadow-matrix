/**
 * Functions that solves matrices multiplication with shadow problem
 * @author: Karolina Kasperek
 */

/*********
 * structs
 *********/
typedef struct {
    unsigned int rows; /* number of rows */
    unsigned int cols; /* number of columns */
    float *data;       /* matrix data row-wise [R0C0, R0C1, R0C2, …, RnC(m-2), RnC(m-1)] */
} matrix_t;

typedef struct {
    float *data;
} shd_t;

/***********
 * functions
 ***********/

/**
 * Get matrix` shadow
 *
 * A – pointer to matrix
 * shd – pointer to final shadow structure
 *
 * Returns percentage of zero-fields covered by the shadow (rounded down)
 * or -1 if coverage is extactly 0%.
 */
int matrix_getshd(const matrix_t *A, shd_t *shd);

/**
* Multiplies C = AB with optional shadows of A and B
*
* A – pointer to matrix A
* B – pointer to matrix B
* C – pointer to final matrix C
* shdA – pointer to A`s shadow (NULL if not available)
* shdB – pointer to B`s shadow (NULL if not available)
*
* Returns 0 on multiplication success,
* or -1 on multiplication failure.
*/
int matrix_prodshd(const matrix_t *A, const matrix_t *B, const shd_t *shdA, shd_t *shdB, matrix_t *C);

/******************
 * helper functions
 ******************/

