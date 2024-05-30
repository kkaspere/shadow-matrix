
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include "matrix_shd.h"
#include "test.h"

static void check_coverage(void **state) {
    mult_teststate_t *teststate = *state;
    assert_int_equal(matrix_getshd(teststate->matrix_A, teststate->shd_A), teststate->coverage_ref);
}

static void check_matrix_shadow(void **state) {

}

static void check_multiplication_shd(void **state) {

    int mult_res;

    mult_teststate_t *teststate = *state;
    matrix_t *output_shd = malloc(sizeof(matrix_t));
    teststate->matrix_C_shd = output_shd;

    unsigned int output_rows_num = teststate->matrix_C_ref->rows;
    unsigned int output_cols_num = teststate->matrix_C_ref->cols;

    mult_res = mult_matrix_with_shd(teststate->matrix_A, teststate->matrix_B, teststate->shd_A_ref,
                                    teststate->shd_B_ref, output_shd);

    assert_int_equal(mult_res, teststate->mult_result_ref);

    if (mult_res == 0) {

        assert_int_equal(output_shd->rows, teststate->matrix_C_ref->rows);
        assert_int_equal(output_shd->cols, teststate->matrix_C_ref->cols);

        for (int row = 0; row < output_rows_num; row++) {
            for (int col = 0; col < output_cols_num; col++) {
                assert_float_equal(output_shd->data[row * output_cols_num + col],
                                   teststate->matrix_C_ref->data[row * output_cols_num + col], 1e-6);
            }
        }
    }
}

static void check_multiplication_naive(void **state) {

    int mult_res;

    mult_teststate_t *teststate = *state;
    matrix_t *output_naive = malloc(sizeof(matrix_t));
    teststate->matrix_C_naive = output_naive;

    unsigned int output_rows_num = teststate->matrix_C_ref->rows;
    unsigned int output_cols_num = teststate->matrix_C_ref->cols;

    mult_res = mult_matrix_naive(teststate->matrix_A, teststate->matrix_B, output_naive);

    assert_int_equal(mult_res, teststate->mult_result_ref);

    if (mult_res == 0) {

        assert_int_equal(output_naive->rows, teststate->matrix_C_ref->rows);
        assert_int_equal(output_naive->cols, teststate->matrix_C_ref->cols);

        for (int row = 0; row < output_rows_num; row++) {
            for (int col = 0; col < output_cols_num; col++) {
                assert_float_equal(output_naive->data[row * output_cols_num + col],
                                   teststate->matrix_C_ref->data[row * output_cols_num + col], 1e-6);
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

    mult_shd_res = matrix_prodshd(teststate->matrix_A, teststate->matrix_B, teststate->shd_A_ref, teststate->shd_B_ref,
                                  output_shd);
    mult_naive_res = matrix_prodshd(teststate->matrix_A, teststate->matrix_B, NULL, NULL, output_naive);
    assert_int_equal(mult_shd_res, mult_naive_res);
    if (mult_shd_res == 0) {
        assert_int_equal(output_shd->rows, output_naive->rows);
        assert_int_equal(output_shd->cols, output_naive->cols);

        for (int row = 0; row < output_shd->rows; row++) {
            for (int col = 0; col < output_shd->cols; col++) {
                assert_float_equal(output_shd->data[row * output_shd->cols + col],
                                   output_naive->data[row * output_naive->cols + col], 1e-6);
            }
        }
    }
}

static int coverage_teardown(void **state) {

    mult_teststate_t *teststate = *state;
    shd_node *head;
    shd_node *head_tmp;

    for (int row = 0; row < teststate->matrix_A->rows; row++) {
        head = teststate->shd_A->shd_data[row];
        while (head != NULL) {
            head_tmp = head;
            head = head->next;
            free(head_tmp);
        }
    }
    free(teststate->shd_A->shd_data);
    free(teststate->shd_A);
    free(teststate);
    return 0;
}

static int mult_shd_teardown(void **state) {

    mult_teststate_t *teststate = *state;
    shd_node *head;
    shd_node *head_tmp;

    for (int row = 0; row < teststate->matrix_A->rows; row++) {
        head = teststate->shd_A_ref->shd_data[row];
        while (head != NULL) {
            head_tmp = head;
            head = head->next;
            free(head_tmp);
        }
    }

    for (int row = 0; row < teststate->matrix_B->rows; row++) {
        head = teststate->shd_B_ref->shd_data[row];
        while (head != NULL) {
            head_tmp = head;
            head = head->next;
            free(head_tmp);
        }
    }
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

int main(void) {
    /**
     * coverage data
     */
    mult_teststate_t *teststate_cov0 = calloc(1, sizeof(mult_teststate_t));
    teststate_cov0->shd_A = malloc(sizeof(shd_t));
    teststate_cov0->matrix_A = &test_matrix_1;
    teststate_cov0->coverage_ref = coverage_expected[0];

    mult_teststate_t *teststate_cov1 = calloc(1, sizeof(mult_teststate_t));
    teststate_cov1->shd_A = malloc(sizeof(shd_t));
    teststate_cov1->matrix_A = &test_matrix_2;
    teststate_cov1->coverage_ref = coverage_expected[1];

    mult_teststate_t *teststate_cov2 = calloc(1, sizeof(mult_teststate_t));
    teststate_cov2->shd_A = malloc(sizeof(shd_t));
    teststate_cov2->matrix_A = &test_matrix_3;
    teststate_cov2->coverage_ref = coverage_expected[2];

    mult_teststate_t *teststate_cov3 = calloc(1, sizeof(mult_teststate_t));
    teststate_cov3->shd_A = malloc(sizeof(shd_t));
    teststate_cov3->matrix_A = &test_matrix_4;
    teststate_cov3->coverage_ref = coverage_expected[3];

    mult_teststate_t *teststate_cov4 = calloc(1, sizeof(mult_teststate_t));
    teststate_cov4->shd_A = malloc(sizeof(shd_t));
    teststate_cov4->matrix_A = &test_matrix_5;
    teststate_cov4->coverage_ref = coverage_expected[4];
    /**
     * multiplication with shd data
     */
    mult_teststate_t *teststate_shd0 = calloc(1, sizeof(mult_teststate_t));

    teststate_shd0->matrix_A = &test_matrix_1A;
    teststate_shd0->matrix_B = &test_matrix_1B;
    teststate_shd0->shd_A_ref = malloc(sizeof(shd_t));
    teststate_shd0->shd_B_ref = malloc(sizeof(shd_t));

    teststate_shd0->shd_A_ref->shd_data = malloc(test_matrix_1A.rows * sizeof(shd_node *));
    shd_node *shd0_row0_A = NULL;
    shd_node *shd0_row1_A = NULL;
    shd_node *shd0_row2_A = NULL;
    shd_node *shd0_row3_A = NULL;

    append_node(&shd0_row0_A, 1);

    append_node(&shd0_row2_A, 0);
    append_node(&shd0_row2_A, 1);
    append_node(&shd0_row2_A, 2);

    append_node(&shd0_row3_A, 1);

    teststate_shd0->shd_A_ref->shd_data[0] = shd0_row0_A;
    teststate_shd0->shd_A_ref->shd_data[1] = shd0_row1_A;
    teststate_shd0->shd_A_ref->shd_data[2] = shd0_row2_A;
    teststate_shd0->shd_A_ref->shd_data[3] = shd0_row3_A;


    teststate_shd0->shd_B_ref->shd_data = malloc(test_matrix_1B.rows * sizeof(shd_node *));
    shd_node *shd0_row0_B = NULL;
    shd_node *shd0_row1_B = NULL;
    shd_node *shd0_row2_B = NULL;

    append_node(&shd0_row0_B, 0);

    append_node(&shd0_row2_B, 1);

    teststate_shd0->shd_B_ref->shd_data[0] = shd0_row0_B;
    teststate_shd0->shd_B_ref->shd_data[1] = shd0_row1_B;
    teststate_shd0->shd_B_ref->shd_data[2] = shd0_row2_B;

    teststate_shd0->mult_result_ref = mult_results_expected[0];
    teststate_shd0->matrix_C_ref = &matrices_result_ref[0];

    /**
     * multiplication naive data
     */
    mult_teststate_t *teststate_naive0 = calloc(1, sizeof(mult_teststate_t));

    teststate_naive0->matrix_A = &test_matrix_1A;
    teststate_naive0->matrix_B = &test_matrix_1B;
    teststate_naive0->mult_result_ref = mult_results_expected[0];
    teststate_naive0->matrix_C_ref = &matrices_result_ref[0];


    /**
     * multiplication consiscency data
     */
    /* mult_teststate_t *teststate1 = calloc(1, sizeof(mult_teststate_t));
     mult_teststate_t *teststate2 = calloc(1, sizeof(mult_teststate_t));

     teststate1->matrix_A = &test_matrix_1A;
     teststate1->matrix_B = &test_matrix_1B;

     teststate2->matrix_B = &test_matrix_1B;
     teststate2->matrix_A = &test_matrix_1A;*/
    const struct CMUnitTest tests[] = {

            /** Coverage and shadow **/
            cmocka_unit_test_prestate_setup_teardown(check_coverage, test_setup, coverage_teardown, teststate_cov0),
            cmocka_unit_test_prestate_setup_teardown(check_coverage, test_setup, coverage_teardown, teststate_cov1),
            cmocka_unit_test_prestate_setup_teardown(check_coverage, test_setup, coverage_teardown, teststate_cov2),
            cmocka_unit_test_prestate_setup_teardown(check_coverage, test_setup, coverage_teardown, teststate_cov3),
            cmocka_unit_test_prestate_setup_teardown(check_coverage, test_setup, coverage_teardown, teststate_cov4),

            /** Mult: naive and with shadow **/
            cmocka_unit_test_prestate_setup_teardown(check_multiplication_shd, test_setup, mult_shd_teardown,
                                                     teststate_shd0),

            cmocka_unit_test_prestate_setup_teardown(check_multiplication_naive, test_setup, mult_naive_teardown,
                                                     teststate_naive0),
            /** Consiscency mult check **/
            // cmocka_unit_test_setup_teardown(check_multiplication_consiscency, setup, teardown)
            // cmocka_unit_test_prestate_setup_teardown(check_multiplication_consiscency, test_setup,
            //                                         mult_consiscency_teardown, teststate1),
            // cmocka_unit_test_prestate_setup_teardown(check_multiplication_consiscency, test_setup,
            //                                        mult_consiscency_teardown, teststate2)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}