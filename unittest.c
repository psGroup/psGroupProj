#include "minunit.h"

int test_one=2;

MU_TEST(test_check) {
    mu_check(test_one == 1);
}
MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_check);
}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return 0;
}
