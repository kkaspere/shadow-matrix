/**
 * @author: Karolina Kasperek
 */

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

/******************************************************************
 * Data of single matrices for coverage and shadow results testing
 ******************************************************************/

#define T_SINGLE_MATRICES_NUM 5
float data_1[] = {0, 4.4, 0,
                  0, 0, 0,
                  9.9, 3, 1.2,
                  0, 3, 0};
const unsigned int rows_1 = 4;
const unsigned int col_1 = 3;

float data_2[]  = {3.3, 5.5,
                   6.6, 0,
                   0, 0};
const unsigned int rows_2 = 3;
const unsigned int col_2 = 2;

float data_3[] = {0};
const unsigned int rows_3 = 1;
const unsigned int col_3 = 1;

float data_4[]  = {0, 0, 0, 0, 0,
                   1.3, 0, 0, 5.5, 0,
                   0, 0, 0, 0, 0,
                   0, 0, 6.7, 0, 0,
                   0, 0, 0, 0, 0};
const unsigned int rows_4 = 5;
const unsigned int col_4 = 5;

float data_5[] = {};
const unsigned int rows_5 = 0;
const unsigned int col_5 = 0;

const matrix_t test_matrix_1 = {rows_1, col_1, data_1};
const matrix_t test_matrix_2 = {rows_2, col_2, data_2};
const matrix_t test_matrix_3 = {rows_3, col_3, data_3};
const matrix_t test_matrix_4 = {rows_4, col_4, data_4};
const matrix_t test_matrix_5 = {rows_5, col_5, data_5};

const matrix_t matrices[T_SINGLE_MATRICES_NUM] = {test_matrix_1, test_matrix_2,  test_matrix_3,  test_matrix_4, test_matrix_5};
int coverage_expected[T_SINGLE_MATRICES_NUM] = {41, 50, -1, 12, -1};


/***********************************************************
 * Data of pair matrices for matrix multiplication testing
 ***********************************************************/

#define T_PAIR_MATRICES_NUM 2
float data_1A[] = {0, 4.4, 0,
                  0, 0, 0,
                  9.9, 3, 1.2,
                  0, 3, 0};
const unsigned int rows_1A = 4;
const unsigned int cols_1A = 3;

float data_1B[] =  {3.3,0,
                    0, 0,
                    0, 5.0};

const unsigned int rows_1B = 3;
const unsigned int cols_1B = 2;

float data_1C[] = {0, 0,
                   0, 0,
                   32.6700, 6.0000,
                   0, 0};

const unsigned int rows_1C = 4;
const unsigned int cols_1C = 2;


const matrix_t test_matrix_1A = {rows_1A, cols_1A, data_1A};
const matrix_t test_matrix_1B = {rows_1B, cols_1B, data_1B};
const matrix_t test_matrix_1C = {rows_1C, cols_1C, data_1C};

matrix_t matrices_to_mult[T_PAIR_MATRICES_NUM][2] = {{test_matrix_1A, test_matrix_1B}, {test_matrix_1A, test_matrix_1B}};
matrix_t matrices_result_ref[T_PAIR_MATRICES_NUM] = {test_matrix_1C, test_matrix_1C};
int mult_results_expected[T_PAIR_MATRICES_NUM] = {0, 0};