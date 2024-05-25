#include <stdio.h>
#include "matrix_shd.h"

int main(void) {
    float data[] = {0, 4.4, 0, 0, 0, 7.8, 9.9, 0, 1.2};
    unsigned int rows = 3;
    unsigned int col = 3;
    const matrix_t test_matrix = {rows, col, data};

    print_matrix(test_matrix);
    shd_t *shd = (shd_t*) malloc(sizeof(shd_t *));
    int coverage = matrix_getshd(&test_matrix, shd);

    /** Prints out list of lists **/
    for (int i = 0; i <  test_matrix.rows; i++) {
        shd_node *ptr = shd->shd_data[i];
        printf("list %d: ", i);
        while (ptr != NULL) {
            printf("%d -> ", ptr->col);
            ptr = ptr->next;
        }
        printf("\n");
    }

    return 0;
}
