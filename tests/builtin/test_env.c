#include "test_builtin.h"
/*
 * envコマンドのテーブル駆動テスト
 * envの特性：全環境変数を表示
 */
Test(builtin_env, table_driven_tests) {
    printf("Running env test\n");

    // テスト用環境変数の設定
    t_env_var *test_env = env_create_from_envp((char*[]){
        "USER=testuser",
        "HOME=/home/testuser",
        "PATH=/usr/bin:/bin",
        NULL
    });

    int result = ft_env(test_env);

    cr_assert_eq(result, 0, "env command should return 0");

    // 実際の出力テストは実装に合わせて調整が必要
    // 通常は標準出力に環境変数が出力される

    env_free(test_env);
}
