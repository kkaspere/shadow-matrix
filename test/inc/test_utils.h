/**
 * Helper things for unit testing
 *
 * @author: Karolina Kasperek
 */

#include <stdbool.h>
#include "matrix_shd.h"

 /**
  * Struct that reflects state of the test
  */
typedef struct {

    const matrix_t *matrix_A;     /** input matrix A (if test require only 1 input matrix, matrix A is used) **/
    const matrix_t *matrix_B;     /** input matrix B **/

    shd_t *shd_A;                 /** Matrix A calculated shadow **/
    shd_t *shd_B;                 /** Matrix B calculated shadow **/
    matrix_t *matrix_C_shd;       /** Product of matrix multiplication (C=AB) using shd func **/
    matrix_t *matrix_C_naive;     /** Product of matrix multiplication (C=AB) using standard function **/

    // true values:
    shd_t *shd_A_ref;             /** Matrix A reference shadow **/
    shd_t *shd_B_ref;             /** Matrix B reference shadow **/
    matrix_t *matrix_C_ref;       /** Reference product of matrix multiplication (C=AB) **/
    int mult_result_ref;          /** Reference result of matrix multiplication **/
    int coverage_ref;             /** Reference coverage of matrix A **/

} mult_teststate_t;

/**
 * Returns an array of pointers to prestates (mult_teststate_t structs) for check_coverage_and_shd tests
 *
 * cases_num - num of tests to be performed - num of required prestates to alloc
 * test_matrices - array of matrices to be tested
 * coverage_ref - array of reference (expected) coverage for given matrices
 */
mult_teststate_t **get_cov_shd_data(unsigned int cases_num, const matrix_t test_matrices[], const int coverage_ref[]);

/**
 * Returns an array of pointers to prestates (mult_teststate_t structs) for tests related to matrix multiplication
 * (check_multiplication_shd, check_multiplication_naive, check_multiplication_consiscency)
 *
 * shd_flag - indicated if input matrices ref shadow has to be allocated and initialized
 * (in naive multiplication shadow is unnecessary)
 *
 * cases_num - num of tests to be performed - num of required prestates to alloc
 * test_matrices - 2D array of matrices pairs to be tested (to be multiplied)
 * output_ref_matrices - array of reference product of multiplication matrices
 * results_ref - array of reference (expected) multiplication result
 */
mult_teststate_t **
get_mult_data(bool shd_flag, unsigned int cases_num, const matrix_t test_matrices[][2], matrix_t output_ref_matrices[],
              const int results_ref[]);
