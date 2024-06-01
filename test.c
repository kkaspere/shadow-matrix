/**
 * @author: Karolina Kasperek
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include <stdbool.h>

#include "matrix_shd.h"
#include "test.h"

static void check_coverage_and_shd(void **state) {

    mult_teststate_t *teststate = *state;
    unsigned int matrix_rows_num = teststate->matrix_A->rows;

    shd_node *shd_calc_node;
    shd_node *shd_ref_node;

    assert_int_equal(matrix_getshd(teststate->matrix_A, teststate->shd_A), teststate->coverage_ref);

    bool flag_found = false;
    /** In case of matrix has a positive coverage value, check shadow contents **/
    if (teststate->coverage_ref > 0) {
        for (int row = 0; row < matrix_rows_num; row++) {

            /** head of the calculated linked list corresponding to currently checked row **/
            shd_calc_node = teststate->shd_A->shd_data[row];

            while (shd_calc_node != NULL) {
                /** head of the reference linked list corresponding to currently checked row **/
                shd_ref_node = teststate->shd_A_ref->shd_data[row];
                flag_found = false;

                /** Iterating through nodes of reference list to check if calculated node is there **/
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

        for (int row = 0; row < output_shd->rows; row++) {
            for (int col = 0; col < output_shd->cols; col++) {
                assert_float_equal(output_shd->data[row * output_shd->cols + col],
                                   teststate->matrix_C_ref->data[row * C_ref_cols_num + col], 1e-6);
            }
        }
    }
}

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

        for (int row = 0; row < output_naive->rows; row++) {
            for (int col = 0; col < output_naive->cols; col++) {
                assert_float_equal(output_naive->data[row * output_naive->cols + col],
                                   teststate->matrix_C_ref->data[row * C_ref_cols_num + col], 1e-6);
            }
        }
    }

}

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

        for (int row = 0; row < output_rows_num; row++) {
            for (int col = 0; col < output_cols_num; col++) {
                assert_float_equal(output_shd->data[row * output_cols_num + col],
                                   output_naive->data[row * output_cols_num + col], 1e-6);
            }
        }
    }
}

static int coverage_teardown(void **state) {

    mult_teststate_t *teststate = *state;
    shd_node *head;
    shd_node *head_tmp;

    for (int row = 0; row < teststate->matrix_A->rows; row++) {
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

static int mult_shd_teardown(void **state) {

    mult_teststate_t *teststate = *state;
    shd_node *head;
    shd_node *head_tmp;

    for (int row = 0; row < teststate->matrix_A->rows; row++)
        free_list(teststate->shd_A_ref->shd_data[row]);

    for (int row = 0; row < teststate->matrix_B->rows; row++)
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

static int mult_naive_teardown(void **state) {

    mult_teststate_t *teststate = *state;
    free(teststate->matrix_C_naive->data);
    free(teststate->matrix_C_naive);
    free(teststate);
    return 0;
}

static int mult_consiscency_teardown(void **state) {

    mult_teststate_t *teststate = *state;

    for (int row = 0; row < teststate->matrix_A->rows; row++)
        free_list(teststate->shd_A_ref->shd_data[row]);

    for (int row = 0; row < teststate->matrix_B->rows; row++)
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

static int test_setup(void **state) {
    mult_teststate_t *teststate = *state;
    return 0;
}

mult_teststate_t **get_cov_shd_data(unsigned int cases_num, const matrix_t test_matrices[], const int coverage_ref[]) {

    mult_teststate_t **teststates = malloc(cases_num * sizeof(mult_teststate_t *));

    for (int c = 0; c < cases_num; c++) {
        teststates[c] = calloc(1, sizeof(mult_teststate_t));
        teststates[c]->shd_A = malloc(sizeof(shd_t));
        teststates[c]->matrix_A = &test_matrices[c];
        teststates[c]->coverage_ref = coverage_ref[c];
        teststates[c]->shd_A_ref = malloc(sizeof(shd_t));
        teststates[c]->shd_A_ref->shd_data = malloc(test_matrices[c].rows * sizeof(shd_node *));
    }

    /**
     * manually fillup reference shd data contents
     */

    /** 0 teststate **/
    shd_node *cov0_row0_A = NULL;
    shd_node *cov0_row1_A = NULL;
    shd_node *cov0_row2_A = NULL;
    shd_node *cov0_row3_A = NULL;

    append_node(&cov0_row0_A, 1);

    append_node(&cov0_row2_A, 0);
    append_node(&cov0_row2_A, 1);
    append_node(&cov0_row2_A, 2);

    append_node(&cov0_row3_A, 1);

    teststates[0]->shd_A_ref->shd_data[0] = cov0_row0_A;
    teststates[0]->shd_A_ref->shd_data[1] = cov0_row1_A;
    teststates[0]->shd_A_ref->shd_data[2] = cov0_row2_A;
    teststates[0]->shd_A_ref->shd_data[3] = cov0_row3_A;

    /** 1 teststate **/
    shd_node *cov1_row0_A = NULL;
    shd_node *cov1_row1_A = NULL;
    shd_node *cov1_row2_A = NULL;

    append_node(&cov1_row0_A, 0);
    append_node(&cov1_row0_A, 1);

    append_node(&cov1_row1_A, 0);

    teststates[1]->shd_A_ref->shd_data[0] = cov1_row0_A;
    teststates[1]->shd_A_ref->shd_data[1] = cov1_row1_A;
    teststates[1]->shd_A_ref->shd_data[2] = cov1_row2_A;

    /** 2 teststate **/
    // matrix with no coverage
    shd_node *cov2_row0_A = NULL;
    teststates[2]->shd_A_ref->shd_data[0] = cov2_row0_A;

    /** 3 teststate **/
    shd_node *cov3_row0_A = NULL;
    shd_node *cov3_row1_A = NULL;
    shd_node *cov3_row2_A = NULL;
    shd_node *cov3_row3_A = NULL;
    shd_node *cov3_row4_A = NULL;

    append_node(&cov3_row1_A, 0);
    append_node(&cov3_row1_A, 3);

    append_node(&cov3_row3_A, 2);

    teststates[3]->shd_A_ref->shd_data[0] = cov3_row0_A;
    teststates[3]->shd_A_ref->shd_data[1] = cov3_row1_A;
    teststates[3]->shd_A_ref->shd_data[2] = cov3_row2_A;
    teststates[3]->shd_A_ref->shd_data[3] = cov3_row3_A;
    teststates[3]->shd_A_ref->shd_data[4] = cov3_row4_A;

    /** 4 teststate **/
    // empty shadow (0 rows and columns)

    /** 5 teststate **/
    // matrix with no coverage
    shd_node *cov5_row0_A = NULL;
    shd_node *cov5_row1_A = NULL;
    shd_node *cov5_row2_A = NULL;
    shd_node *cov5_row3_A = NULL;

    teststates[5]->shd_A_ref->shd_data[0] = cov5_row0_A;
    teststates[5]->shd_A_ref->shd_data[1] = cov5_row1_A;
    teststates[5]->shd_A_ref->shd_data[2] = cov5_row2_A;
    teststates[5]->shd_A_ref->shd_data[3] = cov5_row3_A;

    return teststates;
}

mult_teststate_t **
get_mult_data(bool shd_flag, unsigned int cases_num, const matrix_t test_matrices[][2], matrix_t output_ref_matrices[],
              const int results_ref[]) {

    mult_teststate_t **teststates = malloc(cases_num * sizeof(mult_teststate_t *));

    for (int c = 0; c < cases_num; c++) {

        teststates[c] = calloc(1, sizeof(mult_teststate_t));
        teststates[c]->matrix_A = &test_matrices[c][0];
        teststates[c]->matrix_B = &test_matrices[c][1];
        teststates[c]->mult_result_ref = results_ref[c];
        teststates[c]->matrix_C_ref = &output_ref_matrices[c];

        if (shd_flag) {
            teststates[c]->shd_A_ref = malloc(sizeof(shd_t));
            teststates[c]->shd_B_ref = malloc(sizeof(shd_t));
            teststates[c]->shd_A_ref->shd_data = malloc(test_matrices[c][0].rows * sizeof(shd_node *));
            teststates[c]->shd_B_ref->shd_data = malloc(test_matrices[c][1].rows * sizeof(shd_node *));
        }
    }

    /**
     *  manually fillup reference shd data contents
     */
    if (shd_flag) {

        /*************
         * 0 teststate
         **************/

        /** A **/
        shd_node *shd0_row0_A = NULL;
        shd_node *shd0_row1_A = NULL;
        shd_node *shd0_row2_A = NULL;
        shd_node *shd0_row3_A = NULL;

        append_node(&shd0_row0_A, 1);

        append_node(&shd0_row2_A, 0);
        append_node(&shd0_row2_A, 1);
        append_node(&shd0_row2_A, 2);

        append_node(&shd0_row3_A, 1);

        teststates[0]->shd_A_ref->shd_data[0] = shd0_row0_A;
        teststates[0]->shd_A_ref->shd_data[1] = shd0_row1_A;
        teststates[0]->shd_A_ref->shd_data[2] = shd0_row2_A;
        teststates[0]->shd_A_ref->shd_data[3] = shd0_row3_A;

        /** B **/

        shd_node *shd0_row0_B = NULL;
        shd_node *shd0_row1_B = NULL;
        shd_node *shd0_row2_B = NULL;

        append_node(&shd0_row0_B, 0);

        append_node(&shd0_row2_B, 1);

        teststates[0]->shd_B_ref->shd_data[0] = shd0_row0_B;
        teststates[0]->shd_B_ref->shd_data[1] = shd0_row1_B;
        teststates[0]->shd_B_ref->shd_data[2] = shd0_row2_B;

        /*************
        * 1 teststate
        **************/

        /** A **/

        shd_node *shd1_row0_A = NULL;
        shd_node *shd1_row1_A = NULL;
        shd_node *shd1_row2_A = NULL;
        shd_node *shd1_row3_A = NULL;

        append_node(&shd1_row0_A, 1);
        append_node(&shd1_row0_A, 3);

        append_node(&shd1_row1_A, 0);
        append_node(&shd1_row1_A, 3);

        append_node(&shd1_row2_A, 0);
        append_node(&shd1_row2_A, 1);
        append_node(&shd1_row2_A, 2);

        append_node(&shd1_row3_A, 1);

        teststates[1]->shd_A_ref->shd_data[0] = shd1_row0_A;
        teststates[1]->shd_A_ref->shd_data[1] = shd1_row1_A;
        teststates[1]->shd_A_ref->shd_data[2] = shd1_row2_A;
        teststates[1]->shd_A_ref->shd_data[3] = shd1_row3_A;

        /** B **/

        shd_node *shd1_row0_B = NULL;
        shd_node *shd1_row1_B = NULL;
        shd_node *shd1_row2_B = NULL;
        shd_node *shd1_row3_B = NULL;

        append_node(&shd1_row0_B, 0);
        append_node(&shd1_row0_B, 2);

        append_node(&shd1_row2_B, 1);
        append_node(&shd1_row2_B, 2);
        append_node(&shd1_row2_B, 3);

        append_node(&shd1_row3_B, 0);
        append_node(&shd1_row3_B, 3);

        teststates[1]->shd_B_ref->shd_data[0] = shd1_row0_B;
        teststates[1]->shd_B_ref->shd_data[1] = shd1_row1_B;
        teststates[1]->shd_B_ref->shd_data[2] = shd1_row2_B;
        teststates[1]->shd_B_ref->shd_data[3] = shd1_row3_B;

        /*************
        * 2 teststate
        **************/

        /** A **/

        shd_node *shd2_row0_A = NULL;
        shd_node *shd2_row1_A = NULL;

        append_node(&shd2_row0_A, 1);

        teststates[2]->shd_A_ref->shd_data[0] = shd2_row0_A;
        teststates[2]->shd_A_ref->shd_data[1] = shd2_row1_A;

        /** B **/
        shd_node *shd2_row0_B = NULL;

        append_node(&shd2_row0_B, 0);
        append_node(&shd2_row0_B, 2);
        append_node(&shd2_row0_B, 4);
        append_node(&shd2_row0_B, 5);

        teststates[2]->shd_B_ref->shd_data[0] = shd2_row0_B;

        /*************
        * 3 teststate
        **************/

        /** A **/
        // empty shadow (0 rows and columns)
        /** B **/
        // empty shadow (0 rows and columns)

        /*************
        * 4 teststate
        **************/

        /** A **/

        shd_node *shd4_row0_A = NULL;
        shd_node *shd4_row1_A = NULL;
        shd_node *shd4_row2_A = NULL;
        shd_node *shd4_row3_A = NULL;
        shd_node *shd4_row4_A = NULL;
        shd_node *shd4_row5_A = NULL;

        append_node(&shd4_row0_A, 2);
        append_node(&shd4_row0_A, 3);
        append_node(&shd4_row0_A, 4);

        append_node(&shd4_row1_A, 0);
        append_node(&shd4_row1_A, 1);
        append_node(&shd4_row1_A, 2);
        append_node(&shd4_row1_A, 3);

        append_node(&shd4_row2_A, 0);
        append_node(&shd4_row2_A, 3);

        append_node(&shd4_row4_A, 0);
        append_node(&shd4_row4_A, 1);
        append_node(&shd4_row4_A, 4);
        append_node(&shd4_row4_A, 5);

        append_node(&shd4_row5_A, 2);
        append_node(&shd4_row5_A, 3);
        append_node(&shd4_row5_A, 4);
        append_node(&shd4_row5_A, 5);

        teststates[4]->shd_A_ref->shd_data[0] = shd4_row0_A;
        teststates[4]->shd_A_ref->shd_data[1] = shd4_row1_A;
        teststates[4]->shd_A_ref->shd_data[2] = shd4_row2_A;
        teststates[4]->shd_A_ref->shd_data[3] = shd4_row3_A;
        teststates[4]->shd_A_ref->shd_data[4] = shd4_row4_A;
        teststates[4]->shd_A_ref->shd_data[5] = shd4_row5_A;

        /** B **/

        shd_node *shd4_row0_B = NULL;
        shd_node *shd4_row1_B = NULL;
        shd_node *shd4_row2_B = NULL;
        shd_node *shd4_row3_B = NULL;
        shd_node *shd4_row4_B = NULL;
        shd_node *shd4_row5_B = NULL;

        append_node(&shd4_row0_B, 0);

        append_node(&shd4_row1_B, 0);

        append_node(&shd4_row2_B, 0);

        append_node(&shd4_row3_B, 0);

        append_node(&shd4_row5_B, 0);

        teststates[4]->shd_B_ref->shd_data[0] = shd4_row0_B;
        teststates[4]->shd_B_ref->shd_data[1] = shd4_row1_B;
        teststates[4]->shd_B_ref->shd_data[2] = shd4_row2_B;
        teststates[4]->shd_B_ref->shd_data[3] = shd4_row3_B;
        teststates[4]->shd_B_ref->shd_data[4] = shd4_row4_B;
        teststates[4]->shd_B_ref->shd_data[5] = shd4_row5_B;

        /*************
        * 5 teststate
        **************/

        /** A **/
        // matrix with no coverage
        shd_node *shd5_row0_A = NULL;
        shd_node *shd5_row1_A = NULL;
        shd_node *shd5_row2_A = NULL;

        teststates[5]->shd_A_ref->shd_data[0] = shd5_row0_A;
        teststates[5]->shd_A_ref->shd_data[1] = shd5_row1_A;
        teststates[5]->shd_A_ref->shd_data[2] = shd5_row2_A;

        /** B **/
        // matrix with no coverage
        shd_node *shd5_row0_B = NULL;
        shd_node *shd5_row1_B = NULL;
        shd_node *shd5_row2_B = NULL;

        teststates[5]->shd_B_ref->shd_data[0] = shd5_row0_B;
        teststates[5]->shd_B_ref->shd_data[1] = shd5_row1_B;
        teststates[5]->shd_B_ref->shd_data[2] = shd5_row2_B;
    }

    return teststates;
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
            cmocka_unit_test_prestate_setup_teardown(check_coverage_and_shd, test_setup, coverage_teardown,
                                                     cov_shd_test_states[0]),
            cmocka_unit_test_prestate_setup_teardown(check_coverage_and_shd, test_setup, coverage_teardown,
                                                     cov_shd_test_states[1]),
            cmocka_unit_test_prestate_setup_teardown(check_coverage_and_shd, test_setup, coverage_teardown,
                                                     cov_shd_test_states[2]),
            cmocka_unit_test_prestate_setup_teardown(check_coverage_and_shd, test_setup, coverage_teardown,
                                                     cov_shd_test_states[3]),
            cmocka_unit_test_prestate_setup_teardown(check_coverage_and_shd, test_setup, coverage_teardown,
                                                     cov_shd_test_states[4]),
            cmocka_unit_test_prestate_setup_teardown(check_coverage_and_shd, test_setup, coverage_teardown,
                                                     cov_shd_test_states[5]),

            /** Mult: naive and with shadow **/
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_shd, test_setup, mult_shd_teardown,
                                                     mult_shd_test_states[0]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_shd, test_setup, mult_shd_teardown,
                                                     mult_shd_test_states[1]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_shd, test_setup, mult_shd_teardown,
                                                     mult_shd_test_states[2]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_shd, test_setup, mult_shd_teardown,
                                                     mult_shd_test_states[3]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_shd, test_setup, mult_shd_teardown,
                                                     mult_shd_test_states[4]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_shd, test_setup, mult_shd_teardown,
                                                     mult_shd_test_states[5]),

            cmocka_unit_test_prestate_setup_teardown(check_multiplication_naive, test_setup, mult_naive_teardown,
                                                     mult_naive_test_states[0]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_naive, test_setup, mult_naive_teardown,
                                                     mult_naive_test_states[1]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_naive, test_setup, mult_naive_teardown,
                                                     mult_naive_test_states[2]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_naive, test_setup, mult_naive_teardown,
                                                     mult_naive_test_states[3]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_naive, test_setup, mult_naive_teardown,
                                                     mult_naive_test_states[4]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_naive, test_setup, mult_naive_teardown,
                                                     mult_naive_test_states[5]),
            /** Consiscency mult check **/
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_consiscency, test_setup,
                                                     mult_consiscency_teardown, mult_consiscency_test_states[0]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_consiscency, test_setup,
                                                     mult_consiscency_teardown, mult_consiscency_test_states[1]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_consiscency, test_setup,
                                                     mult_consiscency_teardown, mult_consiscency_test_states[2]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_consiscency, test_setup,
                                                     mult_consiscency_teardown, mult_consiscency_test_states[3]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_consiscency, test_setup,
                                                     mult_consiscency_teardown, mult_consiscency_test_states[4]),
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_consiscency, test_setup,
                                                     mult_consiscency_teardown, mult_consiscency_test_states[5]),
    };
    int tests_res = cmocka_run_group_tests(tests, NULL, NULL);

    free(cov_shd_test_states);
    free(mult_shd_test_states);
    free(mult_naive_test_states);
    free(mult_consiscency_test_states);

    return tests_res;
}