/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtin.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tatsato <tatsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 10:56:54 by tatsato           #+#    #+#             */
/*   Updated: 2025/06/01 18:02:14 by tatsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef TEST_BUILTIN_H
#define TEST_BUILTIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <criterion/criterion.h>
#include "../env/test_env_helpers.h"
#include "usecase/builtin/builtin_commands.h"
#include "domain/token.h"
#include "domain/env_variable.h"
#include "domain/env_initializer.h"


// テストケース用の構造体定義
typedef struct {
    char *test_name;
    char **argv;
    int expected_return;
    char *expected_output;
    expected_env_entry *expected_env;
    int expected_env_count;
} builtin_test_case;
#endif
