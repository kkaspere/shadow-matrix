/**
 * @author: Karolina Kasperek
 */

#include "matrix_shd.h"

int matrix_getshd(const matrix_t *A, shd_t *shd) {

    const unsigned int elements_num = A->rows * A->cols; /** number of elements in matrix **/
    int coverage;                                        /** stores the matrix coverage (in %) **/
    unsigned int nonzero_num = 0;                        /** stores number of non-zero elements in matrix **/
    unsigned int idx;                                    /** stores index of currently processing element **/

    /** Filling up the list for each row **/
    shd->shd_data = (shd_node **) malloc(A->rows * sizeof(shd_node *));

    if (!shd->shd_data) {
        printf("Could not allocate memory, exiting\n");
        exit(2);
    }

    for (int row = 0; row < A->rows; row++) {
        shd_node *list = NULL;
        for (int col = 0; col < A->cols; col++) {
            idx = row * A->cols + col;
            // the author is aware that in real scenario there may be a necessity to check if
            // A->data[idx] is different from 0 with the usage of some eps margin
            // (due to floating point arithmetic accuracy)
            if (A->data[idx] != 0) {
                nonzero_num += 1;
                append_node(&list, col);
            }
            shd->shd_data[row] = list;
        }
    }

    /** Coverage calculation **/
    if (nonzero_num == 0)
        coverage = -1;
    else
        coverage = (nonzero_num * 100) / elements_num;

    return coverage;
}

int matrix_prodshd(const matrix_t *A, const matrix_t *B, const shd_t *shdA, const shd_t *shdB, matrix_t *C) {

    if (shdA == NULL || shdB == NULL)
        return mult_matrix_naive(A, B, C);
    else
        return mult_matrix_with_shd(A, B, shdA, shdB, C);
}

int mult_matrix_naive(const matrix_t *A, const matrix_t *B, matrix_t *C) {

    /** checking if matrix dim are compatible **/
    if (A->cols != B->rows) {
        C->data = NULL;
        return -1;
    }

    unsigned int idx_A;     /** stores index of currently using element from matrix A **/
    unsigned int idx_B;     /** stores index of currently using element from matrix B **/
    unsigned int idx_C;     /** stores index of currently calculating element of result matrix **/

    /** Initialize output matrix **/
    C->rows = A->rows;
    C->cols = B->cols;
    C->data = calloc(C->rows * C->cols, sizeof(float));

    if (!C->data) {
        printf("Could not allocate memory, exiting\n");
        exit(2);
    }

    /** Calculating contents of output matrix from the formula **/
    for (int row = 0; row < C->rows; row++) {
        for (int col = 0; col < C->cols; col++) {
            for (int i = 0; i < A->cols; i++) {

                idx_A = row * A->cols + i;
                idx_B = i * B->cols + col;
                idx_C = row * C->cols + col;

                C->data[idx_C] += A->data[idx_A] * B->data[idx_B];
            }
        }
    }
    return 0;
}

int mult_matrix_with_shd(const matrix_t *A, const matrix_t *B, const shd_t *shdA, const shd_t *shdB, matrix_t *C) {

    /** checking if matrix dim are compatible **/
    if (A->cols != B->rows) {
        C->data = NULL;
        return -1;
    }

    unsigned int idx_A;     /** stores index of currently using element from matrix A **/
    unsigned int idx_B;     /** stores index of currently using element from matrix B **/
    unsigned int idx_C;     /** stores index of currently calculating element of result matrix **/

    float val_A;            /** stores the currently using value from matrix A **/
    float val_B;            /** stores the currently using value from matrix B **/

    shd_node *ptrA = NULL;  /** stores pointer to currently processing node of matrix' A shadow **/
    shd_node *ptrB = NULL;  /** stores pointer to currently processing node of matrix' B shadow **/

    /** Initialize output matrix **/
    C->rows = A->rows;
    C->cols = B->cols;
    C->data = calloc(C->rows * C->cols, sizeof(float));

    if (!C->data) {
        printf("Could not allocate memory, exiting\n");
        exit(2);
    }

    for (int row = 0; row < A->rows; row++) {
        /** Going through the shadow of each matrix A row
         * (columns indexes of non-zero elements) **/
        ptrA = shdA->shd_data[row];
        while (ptrA != NULL) {
            /** Going through the shadow of matrix B row corresponding to recent column idx from matrix A **/
            ptrB = shdB->shd_data[ptrA->col];
            while (ptrB != NULL) {

                /** Calculating output matrix contents in following way:
                * we operate on (row, ptrB->col) element of the output matrix
                * and adds to them the product of val_A and val_B, which are:
                * val_A - (row, ptrA->col) element of A matrix
                * val_B - (ptrA->col, ptrB->col) element of A matrix
                *
                * This approach actually do multiplication operations same as formula determines,
                * but also necessary ones thanks to the shadow usage **/

                idx_A = row * A->cols + ptrA->col;
                idx_B = ptrA->col * B->cols + ptrB->col;
                idx_C = row * C->cols + ptrB->col;

                val_A = A->data[idx_A];
                val_B = B->data[idx_B];

                C->data[idx_C] += val_A * val_B;

                ptrB = ptrB->next;
            }
            ptrA = ptrA->next;
        }
    }
    return 0;
}

void print_matrix(const matrix_t A) {

    for (int i = 0; i < A.rows; i++) {
        for (int j = 0; j < A.cols; j++)
            printf("%f ", A.data[i * A.cols + j]);
        printf("\n");
    }
}

void print_shadow(shd_t *shd_m, unsigned int rows_num) {

    for (int row = 0; row < rows_num; row++) {
        shd_node *shd_node = shd_m->shd_data[row];
        printf("row %d: ", row);
        while (shd_node != NULL) {
            printf("%d -> ", shd_node->col);
            shd_node = shd_node->next;
        }
        printf("\n");
    }
}

void append_node(shd_node **head_node, int data) {

    shd_node *new_node = (shd_node *) malloc(sizeof(shd_node));
    if (!new_node) {
        printf("Could not allocate memory, exiting\n");
        exit(2);
    }
    new_node->col = data;
    new_node->next = *head_node;
    *head_node = new_node;
}

void free_list(shd_node *head_node) {

    shd_node *head_tmp;
    while (head_node != NULL) {
        head_tmp = head_node;
        head_node = head_node->next;
        free(head_tmp);
    }
}
