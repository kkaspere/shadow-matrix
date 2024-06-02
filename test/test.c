/**
 * Unit tests for multiplication with shadow task
 *
 * @author: Karolina Kasperek
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "test_utils.h"
#include "test_data.h"

/** Tests **/

/**
 * Test of matrix_getshd function, checks if calculated matrix' coverage
 * and shadow are correnct
 */
static void check_coverage_and_shd(void **state) {

    mult_teststate_t *teststate = *state;
    unsigned int matrix_rows_num = teststate->matrix_A->rows;

    shd_node *shd_calc_node;
    shd_node *shd_ref_node;

    assert_int_equal(matrix_getshd(teststate->matrix_A, teststate->shd_A), teststate->coverage_ref);

    bool flag_found = false;
    /** In case of matrix has a positive coverage value, check shadow contents **/
    if (teststate->coverage_ref > 0) {
        for (int row = 0; row < matrix_rows_num; ++row) {

            /** first element of the calculated linked list corresponding to currently checked row **/
            shd_calc_node = teststate->shd_A->shd_data[row];

            /** Checking every node of calculated shadow **/
            while (shd_calc_node != NULL) {
                /** head of the reference linked list corresponding to currently checked row **/
                shd_ref_node = teststate->shd_A_ref->shd_data[row];
                flag_found = false;

                /** Iterating through nodes of reference list to check if calculated node, that is being currently checked,
                 * is there **/
                while (shd_ref_node != NULL) {
                    /** Calculated node found in reference list **/
                    if (shd_ref_node->col == shd_ref_node->col) {
                        flag_found = true;
                        break;
                    }
                    shd_ref_node = shd_ref_node->next;
                }
                assert_true(flag_found);
                shd_calc_node = shd_calc_node->next;
            }
        }
    }
}

/**
 * Checks multiplication with shadow functionality
 * Matrices' shadows are treated as an input data for this test - reference values of shd are used
 */
static void check_multiplication_shd(void **state) {

    int mult_res;

    mult_teststate_t *teststate = *state;
    matrix_t *output_shd = malloc(sizeof(matrix_t));
    teststate->matrix_C_shd = output_shd;

    unsigned int C_ref_cols_num = teststate->matrix_C_ref->cols;

    mult_res = mult_matrix_with_shd(teststate->matrix_A, teststate->matrix_B, teststate->shd_A_ref,
                                    teststate->shd_B_ref, output_shd);

    assert_int_equal(mult_res, teststate->mult_result_ref);

    /** If multiplication was successful, check contents of output matrix **/
    if (mult_res == 0) {

        assert_int_equal(output_shd->rows, teststate->matrix_C_ref->rows);
        assert_int_equal(output_shd->cols, teststate->matrix_C_ref->cols);

        for (int row = 0; row < output_shd->rows; ++row) {
            for (int col = 0; col < output_shd->cols; ++col) {
                assert_float_equal(output_shd->data[row * output_shd->cols + col],
                                   teststate->matrix_C_ref->data[row * C_ref_cols_num + col], 1e-6);
            }
        }
    }
}

/**
 * Checks naive multiplication functionality
 */
static void check_multiplication_naive(void **state) {

    int mult_res;

    mult_teststate_t *teststate = *state;
    matrix_t *output_naive = malloc(sizeof(matrix_t));
    teststate->matrix_C_naive = output_naive;

    unsigned int C_ref_cols_num = teststate->matrix_C_ref->cols;

    mult_res = mult_matrix_naive(teststate->matrix_A, teststate->matrix_B, output_naive);

    assert_int_equal(mult_res, teststate->mult_result_ref);

    /** If multiplication was successful, check contents of output matrix**/
    if (mult_res == 0) {

        assert_int_equal(output_naive->rows, teststate->matrix_C_ref->rows);
        assert_int_equal(output_naive->cols, teststate->matrix_C_ref->cols);

        for (int row = 0; row < output_naive->rows; ++row) {
            for (int col = 0; col < output_naive->cols; ++col) {
                assert_float_equal(output_naive->data[row * output_naive->cols + col],
                                   teststate->matrix_C_ref->data[row * C_ref_cols_num + col], 1e-6);
            }
        }
    }

}

/**
 * Checks whether naive and shd multiplication gives same result
 */
static void check_multiplication_consiscency(void **state) {

    int mult_shd_res, mult_naive_res;

    mult_teststate_t *teststate = *state;
    matrix_t *output_shd = malloc(sizeof(matrix_t));
    matrix_t *output_naive = malloc(sizeof(matrix_t));
    teststate->matrix_C_naive = output_naive;
    teststate->matrix_C_shd = output_shd;

    unsigned int output_rows_num = teststate->matrix_C_ref->rows;
    unsigned int output_cols_num = teststate->matrix_C_ref->cols;

    mult_shd_res = matrix_prodshd(teststate->matrix_A, teststate->matrix_B, teststate->shd_A_ref, teststate->shd_B_ref,
                                  output_shd);

    mult_naive_res = matrix_prodshd(teststate->matrix_A, teststate->matrix_B, NULL, NULL, output_naive);

    assert_int_equal(mult_shd_res, mult_naive_res);

    /** If multiplications was successful, check if both output matrices has consistent contents **/
    if (mult_shd_res == 0 && mult_naive_res == 0) {

        assert_int_equal(output_shd->rows, output_naive->rows);
        assert_int_equal(output_shd->cols, output_naive->cols);

        for (int row = 0; row < output_rows_num; ++row) {
            for (int col = 0; col < output_cols_num; ++col) {
                assert_float_equal(output_shd->data[row * output_cols_num + col],
                                   output_naive->data[row * output_cols_num + col], 1e-6);
            }
        }
    }
}

/** Teardown funcs **/

/**
 * Free memory allocated for check_coverage_and_shd test test
 */
static int coverage_and_shd_teardown(void **state) {

    mult_teststate_t *teststate = *state;
    shd_node *head;
    shd_node *head_tmp;

    for (int row = 0; row < teststate->matrix_A->rows; ++row) {
        free_list(teststate->shd_A->shd_data[row]);
        free_list(teststate->shd_A_ref->shd_data[row]);
    }

    free(teststate->shd_A_ref->shd_data);
    free(teststate->shd_A_ref);

    free(teststate->shd_A->shd_data);
    free(teststate->shd_A);
    free(teststate);
    return 0;
}

/**
 * Free memory allocated for check_multiplication_shd test
 */
static int mult_shd_teardown(void **state) {

    mult_teststate_t *teststate = *state;
    shd_node *head;
    shd_node *head_tmp;

    for (int row = 0; row < teststate->matrix_A->rows; ++row)
        free_list(teststate->shd_A_ref->shd_data[row]);

    for (int row = 0; row < teststate->matrix_B->rows; ++row)
        free_list(teststate->shd_B_ref->shd_data[row]);

    free(teststate->shd_A_ref->shd_data);
    free(teststate->shd_A_ref);

    free(teststate->shd_B_ref->shd_data);
    free(teststate->shd_B_ref);

    free(teststate->matrix_C_shd->data);
    free(teststate->matrix_C_shd);

    free(teststate);
    return 0;
}

/**
 * Free memory allocated for check_multiplication_naive test
 */
static int mult_naive_teardown(void **state) {

    mult_teststate_t *teststate = *state;
    free(teststate->matrix_C_naive->data);
    free(teststate->matrix_C_naive);
    free(teststate);
    return 0;
}

/**
 * Free memory allocated for check_multiplication_consiscency test
 */
static int mult_consiscency_teardown(void **state) {

    mult_teststate_t *teststate = *state;

    for (int row = 0; row < teststate->matrix_A->rows; ++row)
        free_list(teststate->shd_A_ref->shd_data[row]);

    for (int row = 0; row < teststate->matrix_B->rows; ++row)
        free_list(teststate->shd_B_ref->shd_data[row]);

    free(teststate->shd_A_ref->shd_data);
    free(teststate->shd_A_ref);

    free(teststate->shd_B_ref->shd_data);
    free(teststate->shd_B_ref);

    free(teststate->matrix_C_shd->data);
    free(teststate->matrix_C_naive->data);

    free(teststate->matrix_C_shd);
    free(teststate->matrix_C_naive);
    free(teststate);
    return 0;
}

int main(void) {
    /** coverage and shd test data **/
    mult_teststate_t **cov_shd_test_states = get_cov_shd_data(SINGLE_MATRICES_NUM, matrices, coverage_expected);
    mult_teststate_t **mult_shd_test_states = get_mult_data(true, PAIR_MATRICES_NUM, matrices_to_mult,
                                                            matrices_result_ref, mult_results_expected);
    mult_teststate_t **mult_naive_test_states = get_mult_data(false, PAIR_MATRICES_NUM, matrices_to_mult,
                                                              matrices_result_ref, mult_results_expected);
    mult_teststate_t **mult_consiscency_test_states = get_mult_data(true, PAIR_MATRICES_NUM, matrices_to_mult,
                                                                    matrices_result_ref, mult_results_expected);

    const struct CMUnitTest tests[] = {

            /** Coverage and shadow **/
            cmocka_unit_test_prestate_setup_teardown(check_coverage_and_shd, NULL, coverage_and_shd_teardown,
                                                     cov_shd_test_states[0]),
            cmocka_unit_test_prestate_setup_teardown(check_coverage_and_shd, NULL, coverage_and_shd_teardown,
                                                     cov_shd_test_states[1]),
            cmocka_unit_test_prestate_setup_teardown(check_coverage_and_shd, NULL, coverage_and_shd_teardown,
                                                     cov_shd_test_states[2]),
            cmocka_unit_test_prestate_setup_teardown(check_coverage_and_shd, NULL, coverage_and_shd_teardown,
                                                     cov_shd_test_states[3]),
            cmocka_unit_test_prestate_setup_teardown(check_coverage_and_shd, NULL, coverage_and_shd_teardown,
                                                     cov_shd_test_states[4]),
            cmocka_unit_test_prestate_setup_teardown(check_coverage_and_shd, NULL, coverage_and_shd_teardown,
                                                     cov_shd_test_states[5]),

            /** Mult: naive and with shadow **/
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_shd, NULL, mult_shd_teardown,
                                                     mult_shd_test_states[0]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_shd, NULL, mult_shd_teardown,
                                                     mult_shd_test_states[1]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_shd, NULL, mult_shd_teardown,
                                                     mult_shd_test_states[2]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_shd, NULL, mult_shd_teardown,
                                                     mult_shd_test_states[3]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_shd, NULL, mult_shd_teardown,
                                                     mult_shd_test_states[4]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_shd, NULL, mult_shd_teardown,
                                                     mult_shd_test_states[5]),

            cmocka_unit_test_prestate_setup_teardown(check_multiplication_naive, NULL, mult_naive_teardown,
                                                     mult_naive_test_states[0]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_naive, NULL, mult_naive_teardown,
                                                     mult_naive_test_states[1]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_naive, NULL, mult_naive_teardown,
                                                     mult_naive_test_states[2]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_naive, NULL, mult_naive_teardown,
                                                     mult_naive_test_states[3]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_naive, NULL, mult_naive_teardown,
                                                     mult_naive_test_states[4]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_naive, NULL, mult_naive_teardown,
                                                     mult_naive_test_states[5]),
            /** Consiscency mult check **/
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_consiscency, NULL,
                                                     mult_consiscency_teardown, mult_consiscency_test_states[0]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_consiscency, NULL,
                                                     mult_consiscency_teardown, mult_consiscency_test_states[1]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_consiscency, NULL,
                                                     mult_consiscency_teardown, mult_consiscency_test_states[2]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_consiscency, NULL,
                                                     mult_consiscency_teardown, mult_consiscency_test_states[3]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_consiscency, NULL,
                                                     mult_consiscency_teardown, mult_consiscency_test_states[4]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_consiscency, NULL,
                                                     mult_consiscency_teardown, mult_consiscency_test_states[5]),

    };

    int tests_res = cmocka_run_group_tests(tests, NULL, NULL);

    free(cov_shd_test_states);
    free(mult_shd_test_states);
    free(mult_naive_test_states);
    free(mult_consiscency_test_states);

    return tests_res;
}