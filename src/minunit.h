/* Adaptation of minunit testing framework from https://jera.com/techinfo/jtns/jtn002 */
#define mu_assert(message, expr) do { if (!(expr)) return message; } while (0)
#define mu_run_test(test) do { \
    char* message = test(); \
    tests_run += 1; \
    if (message) { \
        printf("FAILED: %s\n", message); \
        tests_failed += 1; \
    } \
} while (0)

extern int tests_run;
extern int tests_failed;
