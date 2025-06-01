#include <stdio.h>
#include <string.h>
#include <criterion/criterion.h>
#include "test_env_helpers.h"

void assert_env_equals(t_env_var *env, expected_env_entry *expected, int expected_count) {
    for (int i = 0; i < expected_count; i++) {
        char *actual_value = env_get(env, expected[i].key);
        if (expected[i].value == NULL) {
            cr_assert_null(actual_value, "Expected key '%s' to have NULL value", expected[i].key);
            continue;
        }
        cr_assert_not_null(actual_value, "Expected key '%s' not found", expected[i].key);
        cr_assert_str_eq(actual_value, expected[i].value, "Value mismatch for key '%s'", expected[i].key);
    }
}

void print_env(t_env_var *env) {
    while (env) {
        printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
}
