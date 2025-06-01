#ifndef TEST_ENV_HELPERS_H
#define TEST_ENV_HELPERS_H

#include "domain/env_variable.h"

typedef struct {
    const char *key;
    const char *value;
} expected_env_entry;

void assert_env_equals(t_env_var *env, expected_env_entry *expected, int expected_count);
void print_env(t_env_var *env);

#endif
