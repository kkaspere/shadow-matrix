/**
 * Functions that solves matrices multiplication with shadow problem
 * @author: Karolina Kasperek
 */
#include <stdlib.h>
#include <stdio.h>

/*********
 * structs
 *********/
typedef struct {
    unsigned int rows; /* number of rows */
    unsigned int cols; /* number of columns */
    float *data;       /* matrix data row-wise [R0C0, R0C1, R0C2, …, RnC(m-2), RnC(m-1)] */
} matrix_t;

typedef struct shd_node {
    unsigned int col; /* column index of non-zero element */
    struct shd_node *next;
} shd_node;

typedef struct {
    shd_node **shd_data; /* each list of shd_node type corresponds to the specific row in the matrix (list 0 - row 0 and so on) */
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
int matrix_prodshd(const matrix_t *A, const matrix_t *B, const shd_t *shdA, const shd_t *shdB, matrix_t *C);

/******************
 * helper functions
 ******************/

/**
 * Helper function that prints out the matrix contents
 */
void print_matrix(const matrix_t A);

/**
 * Appends a new node to the linked list
 * head_node - pointer to linked list head node
 * data - data of the new node
 */
void append_node(shd_node **head_node, int data);

/**
 * Free the linked list
 * head_node - pointer to linked list head node
 */
void free_list(shd_node *head_node);

/**
 * Implementation of naive matrix multiplication (from the formula)
 * C = AB
 * Returns 0 on multiplication success,
 * or -1 on multiplication failure.
 */
int mult_matrix_naive(const matrix_t *A,  const matrix_t *B, matrix_t *C);

/**
 * Implementation of matrix multiplication with its shadow
 * C = AB
 * Returns 0 on multiplication success,
 * or -1 on multiplication failure.
 */
int mult_matrix_with_shd(const matrix_t *A, const matrix_t *B, const shd_t *shdA, const shd_t *shdB, matrix_t *C);
