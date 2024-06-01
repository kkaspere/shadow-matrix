/**
 * @author: Karolina Kasperek
 */

#include <stdbool.h>
#include "matrix_shd.h"

typedef struct {
    const matrix_t *matrix_A;     /** input matrix A (if test require only 1 input matrix, matrix A is used) **/
    const matrix_t *matrix_B;     /** input matrix B **/

    shd_t *shd_A;                 /** Matrix A calculated shadow **/
    shd_t *shd_B;                 /** Matrix B calculated shadow **/
    matrix_t *matrix_C_shd;       /** Product of matrix multiplication (C=AB) using shd func **/
    matrix_t *matrix_C_naive;     /** Product of matrix multiplication (C=AB) using standard function **/

    shd_t *shd_A_ref;             /** Matrix A reference shadow **/
    shd_t *shd_B_ref;             /** Matrix B reference shadow **/
    matrix_t *matrix_C_ref;      /** Reference product of matrix multiplication (C=AB) **/
    int mult_result_ref;          /** Expected result of matrix multiplication **/
    int coverage_ref;             /** Reference coverage of matrix A **/
} mult_teststate_t;

mult_teststate_t **get_cov_shd_data(unsigned int cases_num, const matrix_t test_matrices[], const int coverage_ref[]);
mult_teststate_t **
get_mult_data(bool shd_flag, unsigned int cases_num, const matrix_t test_matrices[][2], matrix_t output_ref_matrices[],
              const int results_ref[]);
