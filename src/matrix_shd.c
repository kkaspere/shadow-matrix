/**
 * @author: Karolina Kasperek
 */

#include "matrix_shd.h"

int matrix_getshd(const matrix_t *A, shd_t *shd) {
    shd->shd_data = (shd_node **) malloc(A->rows * sizeof(shd_node *));
    for (int i = 0; i < A->rows; i++)
        shd->shd_data[i] = NULL;

    /** Filling up the lists **/
    for (int i = 0; i < A->rows; i++) {
        shd_node *head = NULL;
        for (int j = 0; j < A->cols; j++) {
            if (A->data[i * A->rows + j] > 0) {
                shd_node *node = (shd_node *) malloc(sizeof(shd_node));
                node->col = j;
                node->next = NULL;
                if (shd->shd_data[i] == NULL) {
                    shd->shd_data[i] = node;
                } else
                    head->next = node;
                head = node;
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