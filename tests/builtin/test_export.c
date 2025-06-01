#include "test_builtin.h"
/*
 * exportコマンドのテーブル駆動テスト
 * exportの特性：環境変数の設定、引数なしで全環境変数表示
 */
Test(builtin_export, table_driven_tests) {
    builtin_test_case test_cases[] = {
        {
            .test_name = "export_new_variable",
            .argv = (char*[]){"export", "NEW_VAR=test_value", NULL},
            .expected_return = 0,
            .expected_output = NULL,
            .expected_env = (expected_env_entry[]){{"NEW_VAR", "test_value"}},
            .expected_env_count = 1
        },
        {
            .test_name = "export_update_existing",
            .argv = (char*[]){"export", "EXISTING=new_value", NULL},
            .expected_return = 0,
            .expected_output = NULL,
            .expected_env = (expected_env_entry[]){{"EXISTING", "new_value"}},
            .expected_env_count = 1
        },
        {
            .test_name = "export_multiple_variables",
            .argv = (char*[]){"export", "VAR1=value1", "VAR2=value2", NULL},
            .expected_return = 0,
            .expected_output = NULL,
            .expected_env = (expected_env_entry[]){
                {"VAR1", "value1"},
                {"VAR2", "value2"}
            },
            .expected_env_count = 2
        },
        {
            .test_name = "export_no_args",
            .argv = (char*[]){"export", NULL},
            .expected_return = 0,
            .expected_output = NULL, // 全環境変数が出力される
            .expected_env = NULL,
            .expected_env_count = 0
        }
    };

    int test_count = sizeof(test_cases) / sizeof(builtin_test_case);

    for (int i = 0; i < test_count; i++) {
        printf("Running export test: %s\n", test_cases[i].test_name);

        // テスト用環境変数の初期化
        t_env_var *test_env = env_create_from_envp((char*[]){
            "EXISTING=old_value",
            "KEEP=keep_value",
            NULL
        });

        int result = ft_export(test_cases[i].argv, &test_env);

        cr_assert_eq(result, test_cases[i].expected_return,
                     "Test '%s': Expected return %d, got %d",
                     test_cases[i].test_name,
                     test_cases[i].expected_return,
                     result);

        // 環境変数の確認
        if (test_cases[i].expected_env) {
            assert_env_equals(test_env, test_cases[i].expected_env,
                            test_cases[i].expected_env_count);
        }

        env_free(test_env);
    }
}
