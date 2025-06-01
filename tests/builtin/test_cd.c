#include "test_builtin.h"
// cd用のテストケース構造体
typedef struct {
    char *test_name;
    char **argv;
    char *initial_pwd;
    int expected_return;
    char *expected_final_dir;
} cd_test_case;

/*
 * cdコマンドのテーブル駆動テスト
 * cdの特性：ディレクトリを変更、PWDとOLDPWD環境変数を更新
 */
Test(builtin_cd, table_driven_tests) {
    char *original_dir = getcwd(NULL, 0);

    cd_test_case test_cases[] = {
        {
            .test_name = "cd_absolute_path",
            .argv = (char*[]){"/var", NULL},
            .initial_pwd = NULL,
            .expected_return = 0,
            .expected_final_dir = "/var"
        },
        // {
        //     .test_name = "cd_relative_path",
        //     .argv = (char*[]){"..", NULL},
        //     .initial_pwd = "/var/test",
        //     .expected_return = 0,
        //     .expected_final_dir = "/var"
        // },
        // {
        //     .test_name = "cd_home",
        //     .argv = (char*[]){NULL}, // 引数なし = HOME
        //     .initial_pwd = NULL,
        //     .expected_return = 0,
        //     .expected_final_dir = NULL // HOMEの値による
        // },
        {
            .test_name = "cd_nonexistent",
            .argv = (char*[]){"cd", "/nonexistent/path", NULL},
            .initial_pwd = NULL,
            .expected_return = 1, // エラー
            .expected_final_dir = NULL // 変更されない
        }
    };

    int test_count = sizeof(test_cases) / sizeof(cd_test_case);

    for (int i = 0; i < test_count; i++) {
        printf("Running cd test: %s\n", test_cases[i].test_name);

        // テスト用環境変数の設定
        t_env_var *test_env = env_create_from_envp((char*[]){
            "HOME=/home/test",
            "PWD=/current",
            "OLDPWD=/previous",
            NULL
        });

        // 初期ディレクトリの設定
        if (test_cases[i].initial_pwd) {
            chdir(test_cases[i].initial_pwd);
        }

        int result = ft_cd(test_cases[i].argv, &test_env);

        cr_assert_eq(result, test_cases[i].expected_return,
                     "Test '%s': Expected return %d, got %d",
                     test_cases[i].test_name,
                     test_cases[i].expected_return,
                     result);

        // 成功した場合のディレクトリ変更確認
        if (result == 0 && test_cases[i].expected_final_dir) {
            char *current_dir = getcwd(NULL, 0);
            cr_assert_str_eq(current_dir, test_cases[i].expected_final_dir,
                           "Test '%s': Directory not changed correctly",
                           test_cases[i].test_name);
            free(current_dir);
        }

        env_free(test_env);
    }

    chdir(original_dir);
    free(original_dir);
}
