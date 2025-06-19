# 出力ファイル名
NAME = minishell

SRCS_DIR = src
INCLUDES_DIR = include
DOMAIN_DIR = $(SRCS_DIR)/domain
USE_CASE_DIR = $(SRCS_DIR)/usecase
ADAPTERS_DIR = $(SRCS_DIR)/adapters
UTILS_DIR = $(SRCS_DIR)/utils

# Domain layer sources
DOMAIN_SRCS	=		$(wildcard $(DOMAIN_DIR)/token/*.c) \
					$(wildcard $(DOMAIN_DIR)/command/*.c)

# Use case layer sources
LEXER_SRCS	=		$(wildcard $(USE_CASE_DIR)/lexer/*.c) \
					$(wildcard $(USE_CASE_DIR)/lexer/handler/*.c)
ENV_SRCS	=		$(wildcard $(USE_CASE_DIR)/env/*.c)
EXECUTOR_SRCS	=	$(wildcard $(USE_CASE_DIR)/executor/*.c)
BUILTIN_SRCS	=	$(wildcard $(USE_CASE_DIR)/builtin/*.c)
ASSIGNMENT_SRCS	=	$(wildcard $(USE_CASE_DIR)/assignment/*.c)
EXIT_SRCS	=		$(wildcard $(USE_CASE_DIR)/exit/*.c)
SIGNAL_SRCS	=		$(wildcard $(USE_CASE_DIR)/signal/*.c)

# Adapters layer sources
ADAPT_CLI_SRCS	=	$(wildcard $(ADAPTERS_DIR)/cli/*.c)
ADAPT_PARSER_SRCS	=	$(wildcard $(ADAPTERS_DIR)/parser/*.c)
ADAPT_SYS_SRCS	=	$(wildcard $(ADAPTERS_DIR)/system/*.c)
ADAPT_IO_SRCS	=	$(wildcard $(ADAPTERS_DIR)/io/*.c)

# Infrastructure layer sources
INFRA_SRCS	=	$(wildcard $(SRCS_DIR)/infrastructure/*.c)

# Utilities
UTILS_SRCS	=		$(wildcard $(UTILS_DIR)/libft_custom/*.c)

# ソースファイル一覧
SRCS			=	$(DOMAIN_SRCS) \
					$(LEXER_SRCS) \
					$(ENV_SRCS) \
					$(EXECUTOR_SRCS) \
					$(BUILTIN_SRCS) \
					$(ASSIGNMENT_SRCS) \
					$(EXIT_SRCS) \
					$(SIGNAL_SRCS) \
					$(ADAPT_CLI_SRCS) \
					$(ADAPT_PARSER_SRCS) \
					$(ADAPT_SYS_SRCS) \
					$(ADAPT_IO_SRCS) \
					$(INFRA_SRCS) \
					$(UTILS_SRCS) \
					$(SRCS_DIR)/main.c
OBJS			=	$(SRCS:%.c=%.o)
LIBFT_DIR		=	$(SRCS_DIR)/utils/libft
LIBFT_A			=	libft.a
# ヘッダファイルのあるディレクトリ
INCLUDES		=	-I$(INCLUDES_DIR) -I$(LIBFT_DIR)
LDFLAGS			=	-lreadline -L$(LIBFT_DIR) -lft
START_COMPILING	=	0

# コンパイルオプション
CFLAGS			=	-Wall -Wextra -Werror -g $(INCLUDES)

# コンパイルコマンド
CC				=	gcc

# 依存関係
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	@echo "\033[0;31m========================================\033[0m\033[0;32m$(@F)\033[0m\033[0;31m========================================\033[0m"
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "Make \033[0;32m$(@F)\033[0m.";
.c.o:
	@if [ $(START_COMPILING) -eq 0 ]; then \
		echo "\033[0;31m==============================\033[0m\033[0;32m$(NAME)'s Objects Compiling\033[0m\033[0;31m==============================\033[0m"; \
		$(eval START_COMPILING := 1) \
	fi
	@$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)
	@echo "Compiling $<."

$(LIBFT_A) :
	@echo "\033[0;31m========================================\033[0m\033[0;32m$(@F)\033[0m\033[0;31m========================================\033[0m"
	@$(MAKE) bonus -C $(LIBFT_DIR)
	@echo "Make \033[0;32m$(@F)\033[0m."

localclean:
	@$(RM) $(OBJS)
	@echo "Removed object files."

clean: localclean
	@$(MAKE) clean -C $(LIBFT_DIR)
	@echo "Cleaned minishell."

fclean: localclean clean_eval
	@$(MAKE) fclean -C $(LIBFT_DIR)
	@$(RM) $(NAME) $(TEST_NAME)
	@echo "Removed executable and tests."

re: clean all

# テスト用ソースと設定
TESTS_DIR		=	tests


TEST_SRCS		=	$(shell find $(TESTS_DIR) -name '*.c' -not -path '$(TESTS_DIR)/parser/*') \
					$(DOMAIN_SRCS) \
					$(LEXER_SRCS) \
					$(ADAPT_PARSER_SRCS) \
					$(EXECUTOR_SRCS) \
					$(BUILTIN_SRCS) \
					$(ASSIGNMENT_SRCS) \
					$(EXIT_SRCS) \
					$(ADAPT_CLI_SRCS) \
					$(ADAPT_SYS_SRCS) \
					$(ADAPT_IO_SRCS) \
					$(UTILS_SRCS)
TEST_OBJS		=	$(TEST_SRCS:%.c=%.o)
TEST_NAME		=	unit_tests
TEST_FLAGS		=	$(CFLAGS) -I$(TESTS_DIR)
TEST_LIBS		=	-lcriterion

# Evaluation test configuration
EVAL_TEST_SRCS	=	$(wildcard $(TESTS_DIR)/evaluation/*.c)
EVAL_TEST_OBJS	=	$(EVAL_TEST_SRCS:%.c=%.o)
EVAL_TEST_NAME	=	evaluation_tests
EVAL_TEST_FLAGS	=	$(CFLAGS) -I$(TESTS_DIR)/evaluation -I$(INCLUDES_DIR)
EVAL_TEST_LIBS	=	-lcriterion

test: $(TEST_NAME)
	@./$(TEST_NAME)

$(TEST_NAME): $(OBJS) $(TEST_OBJS) $(LIBFT_A)
	@echo "\033[0;36mLinking test binary: $(TEST_NAME)\033[0m"
	@$(CC) $(TEST_FLAGS) $(TEST_OBJS) -L$(LIBFT_DIR) -lft $(TEST_LIBS) -o $(TEST_NAME)

test_parser: $(OBJS) test_simple_command test_quote_handling test_pipe test_redirection test_operators test_heredoc

test_simple_command: $(TESTS_DIR)/parser/test_simple_command
	./$(TESTS_DIR)/parser/test_simple_command

test_quote_handling: $(TESTS_DIR)/parser/test_quote_handling
	./$(TESTS_DIR)/parser/test_quote_handling

test_pipe: $(TESTS_DIR)/parser/test_pipe
	./$(TESTS_DIR)/parser/test_pipe

test_redirection: $(TESTS_DIR)/parser/test_redirection
	./$(TESTS_DIR)/parser/test_redirection

test_operators: $(TESTS_DIR)/parser/test_operators
	./$(TESTS_DIR)/parser/test_operators

test_heredoc: $(TESTS_DIR)/parser/test_heredoc
	./$(TESTS_DIR)/parser/test_heredoc

# Comprehensive evaluation test suite for 42 School requirements
test_evaluation: $(EVAL_TEST_NAME)
	@echo "\033[0;32m======== 42 MINISHELL EVALUATION TEST SUITE ========\033[0m"
	@echo "\033[0;33mTesting all mandatory requirements for 100% score...\033[0m"
	@./$(EVAL_TEST_NAME)
	@echo "\033[0;32m======== EVALUATION TESTS COMPLETED ========\033[0m"

$(EVAL_TEST_NAME): $(NAME) $(EVAL_TEST_OBJS)
	@echo "\033[0;36mBuilding evaluation test suite: $(EVAL_TEST_NAME)\033[0m"
	@$(CC) $(EVAL_TEST_FLAGS) $(EVAL_TEST_OBJS) $(EVAL_TEST_LIBS) -o $(EVAL_TEST_NAME)

# Clean evaluation tests
clean_eval:
	@$(RM) $(EVAL_TEST_OBJS) $(EVAL_TEST_NAME)
	@echo "Cleaned evaluation tests"

# Run all tests (unit + evaluation)
test_all: test test_evaluation
	@echo "\033[0;32mAll tests completed successfully!\033[0m"

# Quick evaluation check for CI/CD
test_quick: $(NAME)
	@echo "\033[0;33mRunning quick evaluation check...\033[0m"
	@echo "echo hello" | ./$(NAME) > /dev/null 2>&1 && echo "✅ Basic execution works" || echo "❌ Basic execution failed"
	@echo "export TEST=value && echo \$$TEST" | ./$(NAME) > /dev/null 2>&1 && echo "✅ Variable export works" || echo "❌ Variable export failed"
	@echo "echo test | cat" | ./$(NAME) > /dev/null 2>&1 && echo "✅ Pipes work" || echo "❌ Pipes failed"
	@echo "\033[0;32mQuick check completed\033[0m"

$(TESTS_DIR)/parser/test_simple_command: $(LIBFT_A) $(TESTS_DIR)/parser/test_simple_command.c $(ADAPT_PARSER_SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) $(TESTS_DIR)/parser/test_simple_command.c $(ADAPT_PARSER_SRCS) $(DOMAIN_SRCS) $(UTILS_SRCS) -L$(LIBFT_DIR) -lft -o $@

$(TESTS_DIR)/parser/test_quote_handling: $(LIBFT_A) $(TESTS_DIR)/parser/test_quote_handling.c $(ADAPT_PARSER_SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) $(TESTS_DIR)/parser/test_quote_handling.c $(ADAPT_PARSER_SRCS) $(DOMAIN_SRCS) $(UTILS_SRCS) -L$(LIBFT_DIR) -lft -o $@

$(TESTS_DIR)/parser/test_pipe: $(LIBFT_A) $(TESTS_DIR)/parser/test_pipe.c $(ADAPT_PARSER_SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) $(TESTS_DIR)/parser/test_pipe.c $(ADAPT_PARSER_SRCS) $(DOMAIN_SRCS) $(UTILS_SRCS) -L$(LIBFT_DIR) -lft -o $@

$(TESTS_DIR)/parser/test_redirection: $(LIBFT_A) $(TESTS_DIR)/parser/test_redirection.c $(ADAPT_PARSER_SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) $(TESTS_DIR)/parser/test_redirection.c $(ADAPT_PARSER_SRCS) $(DOMAIN_SRCS) $(UTILS_SRCS) -L$(LIBFT_DIR) -lft -o $@

$(TESTS_DIR)/parser/test_operators: $(LIBFT_A) $(TESTS_DIR)/parser/test_operators.c $(ADAPT_PARSER_SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) $(TESTS_DIR)/parser/test_operators.c $(ADAPT_PARSER_SRCS) $(DOMAIN_SRCS) $(UTILS_SRCS) -L$(LIBFT_DIR) -lft -o $@

$(TESTS_DIR)/parser/test_heredoc: $(LIBFT_A) $(TESTS_DIR)/parser/test_heredoc.c $(ADAPT_PARSER_SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) $(TESTS_DIR)/parser/test_heredoc.c $(ADAPT_PARSER_SRCS) $(DOMAIN_SRCS) $(UTILS_SRCS) -L$(LIBFT_DIR) -lft -o $@

# Integration test for lexer and parser
INTEGRATION_TEST_DIR = $(TESTS_DIR)/integration
COMPREHENSIVE_TEST_SRCS = $(INTEGRATION_TEST_DIR)/comprehensive_test.c \
				   $(DOMAIN_SRCS) \
				   $(LEXER_SRCS) \
				   $(ADAPT_PARSER_SRCS) \
				   $(ENV_SRCS) \
				   $(ASSIGNMENT_SRCS) \
				   $(ADAPT_CLI_SRCS) \
				   $(UTILS_SRCS)
COMPREHENSIVE_TEST_NAME = $(INTEGRATION_TEST_DIR)/comprehensive_test

test_integration: $(COMPREHENSIVE_TEST_NAME)
	@echo "Running comprehensive lexer-parser integration tests..."
	@./$(COMPREHENSIVE_TEST_NAME)

$(COMPREHENSIVE_TEST_NAME): $(LIBFT_A) $(COMPREHENSIVE_TEST_SRCS)
	@mkdir -p $(INTEGRATION_TEST_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(COMPREHENSIVE_TEST_SRCS) -L$(LIBFT_DIR) -lft -o $@

# Executor tests
EXECUTOR_TEST_DIR = $(TESTS_DIR)/executor
EXECUTOR_TEST_BUILTIN_SRCS = $(EXECUTOR_TEST_DIR)/test_builtin_executor.c \
							$(DOMAIN_SRCS) \
							$(LEXER_SRCS) \
							$(ENV_SRCS) \
							$(EXECUTOR_SRCS) \
							$(BUILTIN_SRCS) \
							$(ASSIGNMENT_SRCS) \
							$(EXIT_SRCS) \
							$(UTILS_SRCS)
EXECUTOR_TEST_EXTERNAL_SRCS = $(EXECUTOR_TEST_DIR)/test_external_executor.c \
							 $(DOMAIN_SRCS) \
							 $(LEXER_SRCS) \
							 $(ENV_SRCS) \
							 $(EXECUTOR_SRCS) \
							 $(BUILTIN_SRCS) \
							 $(ASSIGNMENT_SRCS) \
							 $(EXIT_SRCS) \
							 $(UTILS_SRCS)
EXECUTOR_TEST_REDIRECT_SRCS = $(EXECUTOR_TEST_DIR)/test_redirection_executor.c \
							 $(DOMAIN_SRCS) \
							 $(LEXER_SRCS) \
							 $(ENV_SRCS) \
							 $(EXECUTOR_SRCS) \
							 $(BUILTIN_SRCS) \
							 $(ASSIGNMENT_SRCS) \
							 $(EXIT_SRCS) \
							 $(UTILS_SRCS)

EXECUTOR_TEST_BUILTIN_NAME = $(EXECUTOR_TEST_DIR)/test_builtin_executor
EXECUTOR_TEST_EXTERNAL_NAME = $(EXECUTOR_TEST_DIR)/test_external_executor
EXECUTOR_TEST_REDIRECT_NAME = $(EXECUTOR_TEST_DIR)/test_redirection_executor
EXECUTOR_TEST_SIMPLE_SRCS = $(EXECUTOR_TEST_DIR)/test_simple_executor.c \
						   $(DOMAIN_SRCS) \
						   $(LEXER_SRCS) \
						   $(ENV_SRCS) \
						   $(EXECUTOR_SRCS) \
						   $(BUILTIN_SRCS) \
						   $(ASSIGNMENT_SRCS) \
						   $(EXIT_SRCS) \
						   $(UTILS_SRCS)
EXECUTOR_TEST_SIMPLE_NAME = $(EXECUTOR_TEST_DIR)/test_simple_executor

test_executor_builtin: $(EXECUTOR_TEST_BUILTIN_NAME)
	@echo "Running executor builtin tests..."
	@./$(EXECUTOR_TEST_BUILTIN_NAME)

test_executor_external: $(EXECUTOR_TEST_EXTERNAL_NAME)
	@echo "Running executor external tests..."
	@./$(EXECUTOR_TEST_EXTERNAL_NAME)

test_executor_redirection: $(EXECUTOR_TEST_REDIRECT_NAME)
	@echo "Running executor redirection tests..."
	@./$(EXECUTOR_TEST_REDIRECT_NAME)

test_executor_simple: $(EXECUTOR_TEST_SIMPLE_NAME)
	@echo "Running simple executor tests..."
	@./$(EXECUTOR_TEST_SIMPLE_NAME)

test_executor: test_executor_simple test_executor_builtin test_executor_external test_executor_redirection

$(EXECUTOR_TEST_BUILTIN_NAME): $(LIBFT_A) $(EXECUTOR_TEST_BUILTIN_SRCS)
	@mkdir -p $(EXECUTOR_TEST_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(EXECUTOR_TEST_BUILTIN_SRCS) -L$(LIBFT_DIR) -lft -lreadline -o $@

$(EXECUTOR_TEST_EXTERNAL_NAME): $(LIBFT_A) $(EXECUTOR_TEST_EXTERNAL_SRCS)
	@mkdir -p $(EXECUTOR_TEST_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(EXECUTOR_TEST_EXTERNAL_SRCS) -L$(LIBFT_DIR) -lft -lreadline -o $@

$(EXECUTOR_TEST_REDIRECT_NAME): $(LIBFT_A) $(EXECUTOR_TEST_REDIRECT_SRCS)
	@mkdir -p $(EXECUTOR_TEST_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(EXECUTOR_TEST_REDIRECT_SRCS) -L$(LIBFT_DIR) -lft -lreadline -o $@

$(EXECUTOR_TEST_SIMPLE_NAME): $(LIBFT_A) $(EXECUTOR_TEST_SIMPLE_SRCS)
	@mkdir -p $(EXECUTOR_TEST_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(EXECUTOR_TEST_SIMPLE_SRCS) -L$(LIBFT_DIR) -lft -lreadline -o $@

# TDT (Table-Driven Tests) following lexer pattern
PROCESS_TEST_DIR = $(TESTS_DIR)/infrastructure
PROCESS_TDT_SRCS = $(PROCESS_TEST_DIR)/test_process_service_tdt.c \
				   $(TESTS_DIR)/test_utils.c \
				   $(INFRA_SRCS) \
				   $(UTILS_SRCS)
PROCESS_TDT_NAME = $(PROCESS_TEST_DIR)/test_process_service_tdt

REDIRECTION_TDT_SRCS = $(TESTS_DIR)/executor/test_redirection_tdt.c \
					   $(TESTS_DIR)/test_utils.c \
					   src/usecase/executor/input_redirection.c \
					   src/usecase/executor/output_redirection.c \
					   $(INFRA_SRCS) \
					   $(UTILS_SRCS)
REDIRECTION_TDT_NAME = $(TESTS_DIR)/executor/test_redirection_tdt

PIPE_TDT_SRCS = $(TESTS_DIR)/integration/test_pipe_tdt.c \
				$(TESTS_DIR)/test_utils.c \
				$(INFRA_SRCS) \
				$(UTILS_SRCS)
PIPE_TDT_NAME = $(TESTS_DIR)/integration/test_pipe_tdt

# TDT test targets
test_process_tdt: $(PROCESS_TDT_NAME)
	@echo "Running process service TDT tests..."
	@./$(PROCESS_TDT_NAME)

test_redirection_tdt: $(REDIRECTION_TDT_NAME)
	@echo "Running redirection service TDT tests..."
	@./$(REDIRECTION_TDT_NAME)

test_pipe_tdt: $(PIPE_TDT_NAME)
	@echo "Running pipe integration TDT tests..."
	@./$(PIPE_TDT_NAME)

test_all_tdt: test_process_tdt test_redirection_tdt test_pipe_tdt

# TDT build targets
$(PROCESS_TDT_NAME): $(LIBFT_A) $(PROCESS_TDT_SRCS)
	@mkdir -p $(PROCESS_TEST_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(PROCESS_TDT_SRCS) -L$(LIBFT_DIR) -lft -o $@

$(REDIRECTION_TDT_NAME): $(LIBFT_A) $(REDIRECTION_TDT_SRCS)
	@mkdir -p $(TESTS_DIR)/executor
	$(CC) $(CFLAGS) $(INCLUDES) $(REDIRECTION_TDT_SRCS) -L$(LIBFT_DIR) -lft -lreadline -o $@

$(PIPE_TDT_NAME): $(LIBFT_A) $(PIPE_TDT_SRCS)
	@mkdir -p $(INTEGRATION_TEST_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(PIPE_TDT_SRCS) -L$(LIBFT_DIR) -lft -lreadline -o $@

testclean:
	@$(RM) $(TEST_OBJS) $(TEST_NAME) $(COMPREHENSIVE_TEST_NAME)
	@$(RM) $(EXECUTOR_TEST_BUILTIN_NAME) $(EXECUTOR_TEST_EXTERNAL_NAME) $(EXECUTOR_TEST_REDIRECT_NAME)
	@$(RM) $(EXECUTOR_TEST_SIMPLE_NAME)
	@$(RM) $(PROCESS_TDT_NAME) $(REDIRECTION_TDT_NAME) $(PIPE_TDT_NAME)
	@echo "Cleaned test objects and binaries."

.PHONY: all clean fclean re localclean test testclean test_integration test_executor test_executor_simple test_executor_builtin test_executor_external test_executor_redirection test_process_tdt test_redirection_tdt test_pipe_tdt test_all_tdt
