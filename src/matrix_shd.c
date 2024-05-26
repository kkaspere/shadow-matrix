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
    for (int row = 0; row < A->rows; row++) {
        shd_node *list = NULL;
        for (int col = 0; col < A->cols; col++) {
            idx = row * A->cols + col;
            if (A->data[idx] > 0) {
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
        coverage = (nonzero_num*100)/elements_num;

    return coverage;
}

int matrix_prodshd(const matrix_t *A, const matrix_t *B, const shd_t *shdA, shd_t *shdB, matrix_t *C) {

    /** checking if matrix dim are compatible **/
    if (A -> cols != B -> rows)
        return -1;
    C->rows = A->rows;
    C->cols = B->cols;
    C->data = calloc(C->rows*C->cols, sizeof(float));
    if (shdA == NULL || shdB == NULL)
        // do naive multiplication
        printf("Naive mult\n");
    else {
        // multiplication with shadow
        for (int i=0; i < A-> rows; i++) {
            shd_node *ptrA = shdA->shd_data[i];
            printf("idx: %d\n", i);
            while(ptrA != NULL){
                shd_node *ptrB = shdB->shd_data[ptrA->col];
                while(ptrB!=NULL){
                    float val1=A->data[i*A->cols + ptrA->col];
                    float val2 =B->data[ptrA->col*B->cols + ptrB->col];
                    int idx_row = i;
                    int idx_col = ptrB -> col;
                    printf("c[%d][%d] += %.2f * %.2f\n", idx_row, idx_col, val1, val2);
                    C->data[i*C->cols + ptrB -> col] +=  val1*val2 ;
                    ptrB = ptrB -> next;
                }
                ptrA = ptrA->next;
            }
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

void append_node(shd_node **head_node, int data) {
    shd_node *new_node = (shd_node *) malloc(sizeof(shd_node));
    new_node->col = data;
    new_node->next = *head_node;
    *head_node = new_node;
}
