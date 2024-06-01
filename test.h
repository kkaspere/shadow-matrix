/**
 * @author: Karolina Kasperek
 */

#define SINGLE_MATRICES_NUM 6
#define PAIR_MATRICES_NUM 6

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

/** Matrix 0 **/
float data_0[] = {0, 4.4, 0,
                  0, 0, 0,
                  9.9, 3, 1.2,
                  0, 3, 0};
const unsigned int rows_0 = 4;
const unsigned int col_0 = 3;

/** Matrix 1 **/
float data_1[] = {3.3, 5.5,
                  6.6, 0,
                  0, 0};
const unsigned int rows_1 = 3;
const unsigned int col_1 = 2;

/** Matrix 2 **/
float data_2[] = {0};
const unsigned int rows_2 = 1;
const unsigned int col_2 = 1;

/** Matrix 3 **/
float data_3[] = {0, 0, 0, 0, 0,
                  -1.3, 0, 0, 5.5, 0,
                  0, 0, 0, 0, 0,
                  0, 0, -0.1, 0, 0,
                  0, 0, 0, 0, 0};
const unsigned int rows_3 = 5;
const unsigned int col_3 = 5;

/** Matrix 4 **/
float data_4[] = {};
const unsigned int rows_4 = 0;
const unsigned int col_4 = 0;

/** Matrix 5 **/
float data_5[] = {0, 0, 0, 0,
                  0, 0, 0, 0,
                  0, 0, 0, 0,
                  0, 0, 0, 0};
const unsigned int rows_5 = 4;
const unsigned int col_5 = 4;

/** Creating arrays with test data **/
const matrix_t test_matrix_0 = {rows_0, col_0, data_0};
const matrix_t test_matrix_1 = {rows_1, col_1, data_1};
const matrix_t test_matrix_2 = {rows_2, col_2, data_2};
const matrix_t test_matrix_3 = {rows_3, col_3, data_3};
const matrix_t test_matrix_4 = {rows_4, col_4, data_4};
const matrix_t test_matrix_5 = {rows_5, col_5, data_5};

const matrix_t matrices[SINGLE_MATRICES_NUM] = {test_matrix_0, test_matrix_1, test_matrix_2, test_matrix_3,
                                                test_matrix_4, test_matrix_5};
int coverage_expected[SINGLE_MATRICES_NUM] = {41, 50, -1, 12, -1, -1};


/***********************************************************
 * Data of pair matrices for matrix multiplication testing
 ***********************************************************/

/** Pair 0 **/
float data_0A[] = {0, 4.4, 0,
                   0, 0, 0,
                   9.9, 3, 1.2,
                   0, 3, 0};
const unsigned int rows_0A = 4;
const unsigned int cols_0A = 3;

float data_0B[] = {3.3, 0,
                   0, 0,
                   0, 5.0};

const unsigned int rows_0B = 3;
const unsigned int cols_0B = 2;

float data_0C[] = {0, 0,
                   0, 0,
                   32.6700, 6.0000,
                   0, 0};

const unsigned int rows_0C = 4;
const unsigned int cols_0C = 2;

/** Pair 1 **/
float data_1A[] = {0, 4.4, 0, 6.0,
                   1.0, 0, 0, 55.7,
                   9.9, 3.0, 1.2, 0,
                   0, 3.0, 0, 0};
const unsigned int rows_1A = 4;
const unsigned int cols_1A = 4;

float data_1B[] = {3.3, 0, 1.0, 0,
                   0, 0, 0, 0,
                   0, 5.0, 5.5, 77.0,
                   1.0, 0, 0, 101.23};

const unsigned int rows_1B = 4;
const unsigned int cols_1B = 4;

float data_1C[] = {6.0, 0, 0, 607.38,
                   59.0, 0, 1.0, 5638.5110,
                   32.6700, 6.0000, 16.50, 92.40,
                   0, 0, 0, 0};

const unsigned int rows_1C = 4;
const unsigned int cols_1C = 4;

/** Pair 2 **/
float data_2A[] = {0, 4.5,
                   0, 0};
const unsigned int rows_2A = 2;
const unsigned int cols_2A = 2;

float data_2B[] = {3.3, 0, 1.0, 0, 7.0, 7.7};

const unsigned int rows_2B = 1;
const unsigned int cols_2B = 6;

float data_2C[] = {};

const unsigned int rows_2C = 0;
const unsigned int cols_2C = 0;

/** Pair 3 **/
float data_3A[] = {};
const unsigned int rows_3A = 0;
const unsigned int cols_3A = 0;

float data_3B[] = {};

const unsigned int rows_3B = 0;
const unsigned int cols_3B = 0;

float data_3C[] = {};

const unsigned int rows_3C = 0;
const unsigned int cols_3C = 0;

/** Pair 4 **/
float data_4A[] = {0, 0, -55.6, 1.0, 500, 0,
                   9.9, 6.7, -44.0, -44.88, 0, 0,
                   11.0, 0, 0, -9.56, 0, 0,
                   0, 0, 0, 0, 0, 0,
                   300.55, -11.34, 0, 0, 89.6, 4.0,
                   0, 0, 77.8, -101.0, 56.7, 65.0};
const unsigned int rows_4A = 6;
const unsigned int cols_4A = 6;

float data_4B[] = {5.6,
                   7.8,
                   9.9,
                   -55.0,
                   0,
                   7.8};

const unsigned int rows_4B = 6;
const unsigned int cols_4B = 1;

float data_4C[] = {-605.44,
                   2140.50,
                   587.40,
                   0,
                   1625.8280,
                   6832.220};

const unsigned int rows_4C = 6;
const unsigned int cols_4C = 1;

/** Pair 5 **/
float data_5A[] = {0, 0, 0,
                   0, 0, 0,
                   0, 0, 0};
const unsigned int rows_5A = 3;
const unsigned int cols_5A = 3;

float data_5B[] = {0, 0,
                   0, 0,
                   0, 0};

const unsigned int rows_5B = 3;
const unsigned int cols_5B = 2;

float data_5C[] = {0, 0,
                   0, 0,
                   0, 0};

const unsigned int rows_5C = 3;
const unsigned int cols_5C = 2;

/** Creating arrays with test data **/
const matrix_t test_matrix_0A = {rows_0A, cols_0A, data_0A};
const matrix_t test_matrix_0B = {rows_0B, cols_0B, data_0B};
const matrix_t test_matrix_0C = {rows_0C, cols_0C, data_0C};

const matrix_t test_matrix_1A = {rows_1A, cols_1A, data_1A};
const matrix_t test_matrix_1B = {rows_1B, cols_1B, data_1B};
const matrix_t test_matrix_1C = {rows_1C, cols_1C, data_1C};

const matrix_t test_matrix_2A = {rows_2A, cols_2A, data_2A};
const matrix_t test_matrix_2B = {rows_2B, cols_2B, data_2B};
const matrix_t test_matrix_2C = {rows_2C, cols_2C, data_2C};

const matrix_t test_matrix_3A = {rows_3A, cols_3A, data_3A};
const matrix_t test_matrix_3B = {rows_3B, cols_3B, data_3B};
const matrix_t test_matrix_3C = {rows_3C, cols_3C, data_3C};

const matrix_t test_matrix_4A = {rows_4A, cols_4A, data_4A};
const matrix_t test_matrix_4B = {rows_4B, cols_4B, data_4B};
const matrix_t test_matrix_4C = {rows_4C, cols_4C, data_4C};

const matrix_t test_matrix_5A = {rows_5A, cols_5A, data_5A};
const matrix_t test_matrix_5B = {rows_5B, cols_5B, data_5B};
const matrix_t test_matrix_5C = {rows_5C, cols_5C, data_5C};

matrix_t matrices_to_mult[PAIR_MATRICES_NUM][2] = {{test_matrix_0A, test_matrix_0B},
                                                   {test_matrix_1A, test_matrix_1B},
                                                   {test_matrix_2A, test_matrix_2B},
                                                   {test_matrix_3A, test_matrix_3B},
                                                   {test_matrix_4A, test_matrix_4B},
                                                   {test_matrix_5A, test_matrix_5B}};

matrix_t matrices_result_ref[PAIR_MATRICES_NUM] = {test_matrix_0C, test_matrix_1C, test_matrix_2C, test_matrix_3C,
                                                   test_matrix_4C, test_matrix_5C};

int mult_results_expected[PAIR_MATRICES_NUM] = {0, 0, -1, 0, 0, 0};