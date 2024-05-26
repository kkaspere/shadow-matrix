#include <stdio.h>
#include "matrix_shd.h"

int main(void) {
    float data_A[] = {0, 4.4, 0, 0, 0, 0, 9.9, 3, 1.2, 0, 3, 0};
    unsigned int rows_A = 4;
    unsigned int col_A = 3;

    float data_B[]  = {3.3,5.5,6.6, 0, 0,0};

    unsigned int rows_B = 3;
    unsigned int col_B = 2;

    const matrix_t test_matrix_A = {rows_A, col_A, data_A};
    const matrix_t test_matrix_B = {rows_B, col_B, data_B};
    matrix_t *output_matrix = (matrix_t*) malloc(sizeof(matrix_t));
    printf("Test matrix A:\n");
    print_matrix(test_matrix_A);
    shd_t *shd_A = (shd_t*) malloc(sizeof(shd_t));
    shd_t *shd_B = (shd_t*) malloc(sizeof(shd_t));

    int coverageA = matrix_getshd(&test_matrix_A, shd_A);
    int coverageB = matrix_getshd(&test_matrix_B, shd_B);
    printf("Coverage A: %d\n", coverageA);
    printf("Coverage B: %d\n", coverageB);
    /** Prints out list of lists **/
    printf("LIST A\n");
    for (int i = 0; i <  test_matrix_A.rows; i++) {
        shd_node *ptr = shd_A->shd_data[i];
        printf("list %d: ", i);
        while (ptr != NULL) {
            printf("%d -> ", ptr->col);
            ptr = ptr->next;
        }
        printf("\n");
    }
    printf("LIST B\n");
    for (int i = 0; i <  test_matrix_B.rows; i++) {
        shd_node *ptr = shd_B->shd_data[i];
        printf("list %d: ", i);
        while (ptr != NULL) {
            printf("%d -> ", ptr->col);
            ptr = ptr->next;
        }
        printf("\n");
    }

    printf("====\n");
    matrix_prodshd(&test_matrix_A, &test_matrix_B,  shd_A,  shd_B, output_matrix);
    //matrix_prodshd(&test_matrix_A, &test_matrix_B,  NULL,  NULL, output_matrix);
    printf("Output matrix:\n");
    print_matrix(*output_matrix);
    return 0;
}
