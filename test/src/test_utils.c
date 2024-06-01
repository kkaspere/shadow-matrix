#include "test_utils.h"

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
