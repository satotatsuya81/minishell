#include "test_builtin.h"
// pwd用のテストケース構造体
typedef struct {
    char *test_name;
    char *setup_dir;  // テスト前に移動するディレクトリ
    int expected_return;
    char *expected_output_contains; // 出力に含まれるべき文字列
} pwd_test_case;

/*
 * pwdコマンドのテーブル駆動テスト
 * pwdの特性：現在のディレクトリパスを出力
 */
Test(builtin_pwd, table_driven_tests) {
    char *original_dir = getcwd(NULL, 0); // 元のディレクトリを保存

    pwd_test_case test_cases[] = {
        {
            .test_name = "pwd_root",
            .setup_dir = "/",
            .expected_return = 0,
            .expected_output_contains = "/"
        },
        {
            .test_name = "pwd_tmp",
            .setup_dir = "/tmp",
            .expected_return = 0,
            .expected_output_contains = "/tmp"
        },
        {
            .test_name = "pwd_current",
            .setup_dir = NULL, // 現在のディレクトリのまま
            .expected_return = 0,
            .expected_output_contains = NULL // 何かしらのパスが返れば良い
        }
    };

    int test_count = sizeof(test_cases) / sizeof(pwd_test_case);

    for (int i = 0; i < test_count; i++) {
        printf("Running pwd test: %s\n", test_cases[i].test_name);

        // テスト用のディレクトリに移動
        if (test_cases[i].setup_dir) {
            chdir(test_cases[i].setup_dir);
        }

        int result = ft_pwd();

        cr_assert_eq(result, test_cases[i].expected_return,
                     "Test '%s': Expected return %d, got %d",
                     test_cases[i].test_name,
                     test_cases[i].expected_return,
                     result);

        // 実際の出力テストは実装に合わせて調整
    }

    // 元のディレクトリに戻す
    chdir(original_dir);
    free(original_dir);
}
