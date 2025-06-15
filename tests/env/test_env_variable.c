#include <criterion/criterion.h>
#include "domain/env_variable.h"
#include "usecase/env/env_manager.h"
#include "test_env_helpers.h"

Test(env_variable, test_add_get_remove_free) {
    t_env_var *env = NULL;

    // 追加
    env_add(&env, "USER", "alice", "=");
    env_add(&env, "PATH", "/usr/bin", "=");
    env_add(&env, "HOME", "/home/alice", "=");

    expected_env_entry expected1[] = {
        {"USER", "alice"},
        {"PATH", "/usr/bin"},
        {"HOME", "/home/alice"},
    };
    assert_env_equals(env, expected1, 3);

    // 上書き
    env_add(&env, "PATH", "/bin", "=");
    expected_env_entry expected2[] = {
        {"USER", "alice"},
        {"PATH", "/bin"},
        {"HOME", "/home/alice"},
    };
    assert_env_equals(env, expected2, 3);

    // 削除
    env_remove(&env, "USER");
    expected_env_entry expected3[] = {
        {"PATH", "/bin"},
        {"HOME", "/home/alice"},
    };
    assert_env_equals(env, expected3, 2);
    cr_assert_null(env_get(env, "USER"));

    // メモリ解放
    env_free(env);
}

Test(env_variable, test_create_from_envp) {
    char *envp[] = {
        "USER=alice",
        "SHELL=/bin/bash",
        "LANG=en_US.UTF-8",
        NULL
    };

    t_env_var *env = env_create_from_envp(envp);

    expected_env_entry expected[] = {
        {"USER", "alice"},
        {"SHELL", "/bin/bash"},
        {"LANG", "en_US.UTF-8"},
    };
    assert_env_equals(env, expected, 3);

    env_free(env);
}

Test(env_variable, test_null_and_empty_values) {
    t_env_var *env = NULL;

    // NULL key（未定義の動作、ここではスキップまたはクラッシュしない前提）
    env_add(&env, NULL, "somevalue", "=");
    // NULL value
    env_add(&env, "NULL_VALUE", NULL, "=");
    // 空文字 key
    env_add(&env, "", "empty_key", "=");
    // 空文字 value
    env_add(&env, "EMPTY_VALUE", "", "=");

    expected_env_entry expected[] = {
        {"NULL_VALUE", NULL},
        {"", "empty_key"},
        {"EMPTY_VALUE", ""},
    };
	assert_env_equals(env, expected, 3);
    cr_assert_str_eq(env_get(env, ""), "empty_key");
    cr_assert_str_eq(env_get(env, "EMPTY_VALUE"), "");
    cr_assert_null(env_get(env, "NULL_KEY"));  // 存在しない

    env_free(env);
}

Test(env_variable, test_remove_nonexistent_and_boundary_nodes) {
    t_env_var *env = NULL;
    env_add(&env, "A", "1", "=");
    env_add(&env, "B", "2", "=");
    env_add(&env, "C", "3", "=");

    // 存在しないキー削除（クラッシュしない）
    env_remove(&env, "Z");

    // 先頭削除
    env_remove(&env, "A");
    cr_assert_null(env_get(env, "A"));

    // 中間削除
    env_remove(&env, "B");
    cr_assert_null(env_get(env, "B"));

    // 最後削除
    env_remove(&env, "C");
    cr_assert_null(env_get(env, "C"));

    env_free(env);
}

Test(env_variable, test_env_create_from_envp_with_invalid_entry) {
    char *envp[] = {
        "FOO=foo",
        "INVALID_ENTRY",
        "BAR=bar",
        NULL
    };

    t_env_var *env = env_create_from_envp(envp);

    expected_env_entry expected[] = {
        {"FOO", "foo"},
        {"BAR", "bar"},
    };
    assert_env_equals(env, expected, 2);
    cr_assert_null(env_get(env, "INVALID_ENTRY"));

    env_free(env);
}
