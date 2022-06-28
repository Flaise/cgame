#ifdef TEST

#include <stdio.h>
#include "minunit.h"

int tests_run = 0;
int tests_failed = 0;

int foo = 7;
int bar = 4;

static char * test_foo() {
    mu_assert("foo != 7", foo == 7);
    return 0;
}

static char * test_bar() {
    mu_assert("bar != 5", bar == 5);
    return 0;
}

static char* test_r() {
    mu_assert("?", 1);
    return 0;
}

int main(int argc, char **argv) {
    mu_run_test(test_foo);
    mu_run_test(test_bar);
    mu_run_test(test_r);

    printf("Passed: %d Failed: %d\n", tests_run - tests_failed, tests_failed);

    return tests_failed;
}

#endif /* TEST */
