#include "test_builtin.h"
// exit用のテストケース構造体
// typedef struct {
//     char *test_name;
//     char **argv;
//     int expected_return;
// } exit_test_case;


// /*
//  * exitコマンドのテーブル駆動テスト
//  * exitの特性：指定された終了コードでプログラム終了
//  */
// Test(builtin_exit, table_driven_tests) {
//     exit_test_case test_cases[] = {
//         {
//             .test_name = "exit_no_args",
//             .argv = (char*[]){NULL},
//             .expected_return = 0 // デフォルト終了コード
//         },
//         {
//             .test_name = "exit_with_code",
//             .argv = (char*[]){"42", NULL},
//             .expected_return = 42
//         },
//         {
//             .test_name = "exit_with_zero",
//             .argv = (char*[]){"0", NULL},
//             .expected_return = 0
//         }
//         // 注意：実際のexitは不正な引数でエラーを返すかもしれない
//     };

//     int test_count = sizeof(test_cases) / sizeof(exit_test_case);

//     for (int i = 0; i < test_count; i++) {
//         printf("Running exit test: %s\n", test_cases[i].test_name);

//         // 注意：実際のft_exitが本当にexit()を呼ぶ場合、
//         // テストプロセスが終了してしまうため、
//         // 戻り値のみをテストする実装が必要

//         ft_exit(test_cases[i].argv, NULL, NULL);

//         cr_assert_eq(result, test_cases[i].expected_return,
//                      "Test '%s': Expected return %d, got %d",
//                      test_cases[i].test_name,
//                      test_cases[i].expected_return,
//                      result);
//     }
// }
