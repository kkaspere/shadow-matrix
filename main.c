/**
 * Program that demonstrates functionality of matrix multiplication with usage of shadow
 * @author: Karolina Kasperek
 */

#include "matrix_shd.h"

int main(void) {

    /** Input data **/
    float data_A[] = {0, 4.4, 0,
                      0, 0, 0,
                      9.9, 3,
                      1.2, 0, 3, 0};
    unsigned int rows_A = 4;
    unsigned int col_A = 3;

    float data_B[] = {3.3, 0, 0, 0, 5.6,
                      1.2, 5.6, 0, 0, 0,
                      0, 0, 0, 0, 0};
    unsigned int rows_B = 3;
    unsigned int col_B = 5;

    const matrix_t test_matrix_A = {rows_A, col_A, data_A};
    const matrix_t test_matrix_B = {rows_B, col_B, data_B};

    /** Mem allocation **/
    matrix_t *output_matrix = (matrix_t *) malloc(sizeof(matrix_t));
    shd_t *shd_A = (shd_t *) malloc(sizeof(shd_t));
    shd_t *shd_B = (shd_t *) malloc(sizeof(shd_t));

    if (!output_matrix || !shd_A || !shd_B) {
        printf("Could not allocate memory, exiting\n");
        exit(2);
    }

    int coverageA = matrix_getshd(&test_matrix_A, shd_A);
    int coverageB = matrix_getshd(&test_matrix_B, shd_B);
    int mult_res = matrix_prodshd(&test_matrix_A, &test_matrix_B, shd_A, shd_B, output_matrix);

    /** Show output **/
    printf("Matrix multiplication with shadow demo program\n");
    printf("===============================================\n");
    printf("matrix A:\n");
    print_matrix(&test_matrix_A);
    printf("\n");
    printf("A matrix coverage: %d%% \n\n", coverageA);
    printf("A matrix shadow:\n");
    print_shadow(shd_A, test_matrix_A.rows);
    printf("\n");

    printf("matrix B:\n");
    print_matrix(&test_matrix_B);
    printf("\n");
    printf("B matrix coverage: %d%% \n\n", coverageB);
    printf("B matrix shadow:\n");
    print_shadow(shd_B, test_matrix_B.rows);
    printf("\n");

    printf("C matrix (C=AB result):\n");
    print_matrix(output_matrix);
    printf("\n");
    printf("Multiplication result: %d\n", mult_res);

    /** Free allocated mem **/
    free(output_matrix->data);
    free(output_matrix);

    for (int row = 0; row < test_matrix_A.rows; ++row)
        free_list(shd_A->shd_data[row]);

    free(shd_A->shd_data);
    free(shd_A);

    for (int row = 0; row < test_matrix_B.rows; ++row)
        free_list(shd_B->shd_data[row]);

    free(shd_B->shd_data);
    free(shd_B);

    return 0;
}
