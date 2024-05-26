
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include "matrix_shd.h"
#include "test.h"

static void check_coverage_results(void **state) {
    for (int i = 0; i < T_COVERAGE_MATRICES_NUM; i++) {
        shd_t *shd_tmp = (shd_t*) malloc(sizeof(shd_t));
        assert_true(matrix_getshd(&matrices[i], shd_tmp) == coverage_expected[i]);
        printf("Matrix %d OK\n", i);
    }
}

int main(void) {

    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_coverage_results),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}