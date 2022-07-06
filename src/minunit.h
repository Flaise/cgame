/* Adaptation of minunit testing framework from https://jera.com/techinfo/jtns/jtn002 */
#define mu_assert(expr, message) do { if (!(expr)) return #expr " " message; } while (0)

#define mu_run_test(test) do { \
    char* message = test(); \
    tests_run += 1; \
    if (message) { \
        ERROR("Test failed [%s] %s", #test, message); \
        tests_failed += 1; \
    } \
} while (0)

extern int tests_run;
extern int tests_failed;
