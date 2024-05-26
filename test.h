/**************************************************
 * Data for coverage calculation result testing
 ***************************************************/

#define T_COVERAGE_MATRICES_NUM 5
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

const matrix_t matrices[T_COVERAGE_MATRICES_NUM] = {test_matrix_1, test_matrix_2,  test_matrix_3,  test_matrix_4, test_matrix_5};
int coverage_expected[T_COVERAGE_MATRICES_NUM] = {41, 50, -1, 12, -1};