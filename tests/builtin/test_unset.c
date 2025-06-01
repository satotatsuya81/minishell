#include "test_builtin.h"
/*
 * unsetコマンドのテーブル駆動テスト
 * unsetの特性：指定された環境変数を削除
 */
Test(builtin_unset, table_driven_tests) {
    builtin_test_case test_cases[] = {
        {
            .test_name = "unset_existing_variable",
            .argv = (char*[]){"unset", "TO_DELETE", NULL},
            .expected_return = 0,
            .expected_output = NULL,
            .expected_env = (expected_env_entry[]){{"TO_DELETE", NULL}}, // NULL = 存在しない
            .expected_env_count = 1
        },
        {
            .test_name = "unset_nonexistent_variable",
            .argv = (char*[]){"unset", "NONEXISTENT", NULL},
            .expected_return = 0, // 通常はエラーにならない
            .expected_output = NULL,
            .expected_env = NULL,
            .expected_env_count = 0
        },
        {
            .test_name = "unset_multiple_variables",
            .argv = (char*[]){"unset", "VAR1", "VAR2", NULL},
            .expected_return = 0,
            .expected_output = NULL,
            .expected_env = (expected_env_entry[]){
                {"VAR1", NULL},
                {"VAR2", NULL}
            },
            .expected_env_count = 2
        }
    };

    int test_count = sizeof(test_cases) / sizeof(builtin_test_case);

    for (int i = 0; i < test_count; i++) {
        printf("Running unset test: %s\n", test_cases[i].test_name);

        // テスト用環境変数の初期化
        t_env_var *test_env = env_create_from_envp((char*[]){
            "TO_DELETE=will_be_deleted",
            "VAR1=value1",
            "VAR2=value2",
            "KEEP=keep_this",
            NULL
        });

        int result = ft_unset(test_cases[i].argv, &test_env);

        cr_assert_eq(result, test_cases[i].expected_return,
                     "Test '%s': Expected return %d, got %d",
                     test_cases[i].test_name,
                     test_cases[i].expected_return,
                     result);

        // 削除確認
        if (test_cases[i].expected_env) {
            assert_env_equals(test_env, test_cases[i].expected_env,
                            test_cases[i].expected_env_count);
        }

        env_free(test_env);
    }
}
