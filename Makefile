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
					$(wildcard $(DOMAIN_DIR)/command/*.c) \
					$(wildcard $(DOMAIN_DIR)/env/*.c)

# Use case layer sources  
LEXER_SRCS	=		$(wildcard $(USE_CASE_DIR)/lexer/*.c) \
					$(wildcard $(USE_CASE_DIR)/lexer/handler/*.c)
PARSER_SRCS	=		$(wildcard $(USE_CASE_DIR)/parser/*.c)
EXECUTOR_SRCS	=	$(wildcard $(USE_CASE_DIR)/executor/*.c)
BUILTIN_SRCS	=	$(wildcard $(USE_CASE_DIR)/builtin/*.c)
ASSIGNMENT_SRCS	=	$(wildcard $(USE_CASE_DIR)/assignment/*.c)
EXIT_SRCS	=		$(wildcard $(USE_CASE_DIR)/exit/*.c)

# Adapters layer sources
ADAPT_CLI_SRCS	=	$(wildcard $(ADAPTERS_DIR)/cli/*.c)
ADAPT_SYS_SRCS	=	$(wildcard $(ADAPTERS_DIR)/system/*.c)
ADAPT_IO_SRCS	=	$(wildcard $(ADAPTERS_DIR)/io/*.c)

# Utilities
UTILS_SRCS	=		$(wildcard $(UTILS_DIR)/libft_custom/*.c)

# ソースファイル一覧
SRCS			=	$(DOMAIN_SRCS) \
					$(LEXER_SRCS) \
					$(PARSER_SRCS) \
					$(EXECUTOR_SRCS) \
					$(BUILTIN_SRCS) \
					$(ASSIGNMENT_SRCS) \
					$(EXIT_SRCS) \
					$(ADAPT_CLI_SRCS) \
					$(ADAPT_SYS_SRCS) \
					$(ADAPT_IO_SRCS) \
					$(UTILS_SRCS) \
					$(SRCS_DIR)/main.c
OBJS			=	$(SRCS:%.c=%.o)
LIBFT_DIR		=	$(SRCS_DIR)/utils/libft
LIBFT_A			=	libft.a
# ヘッダファイルのあるディレクトリ
INCLUDES		=	-I$(INCLUDES_DIR) -I$(LIBFT_DIR) -L/usr/include -lreadline
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

fclean: localclean
	@$(MAKE) fclean -C $(LIBFT_DIR)
	@$(RM) $(NAME)
	@echo "Removed executable."

re: clean all

# テスト用ソースと設定
TESTS_DIR		=	tests


TEST_SRCS		=	$(shell find $(TESTS_DIR) -name '*.c' -not -path '$(TESTS_DIR)/parser/*') \
					$(DOMAIN_SRCS) \
					$(LEXER_SRCS) \
					$(PARSER_SRCS) \
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

$(TESTS_DIR)/parser/test_simple_command: $(LIBFT_A) $(TESTS_DIR)/parser/test_simple_command.c $(PARSER_SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) $(TESTS_DIR)/parser/test_simple_command.c $(PARSER_SRCS) -L$(LIBFT_DIR) -lft -o $@

$(TESTS_DIR)/parser/test_quote_handling: $(LIBFT_A) $(TESTS_DIR)/parser/test_quote_handling.c $(PARSER_SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) $(TESTS_DIR)/parser/test_quote_handling.c $(PARSER_SRCS) -L$(LIBFT_DIR) -lft -o $@

$(TESTS_DIR)/parser/test_pipe: $(LIBFT_A) $(TESTS_DIR)/parser/test_pipe.c $(PARSER_SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) $(TESTS_DIR)/parser/test_pipe.c $(PARSER_SRCS) -L$(LIBFT_DIR) -lft -o $@

$(TESTS_DIR)/parser/test_redirection: $(LIBFT_A) $(TESTS_DIR)/parser/test_redirection.c $(PARSER_SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) $(TESTS_DIR)/parser/test_redirection.c $(PARSER_SRCS) -L$(LIBFT_DIR) -lft -o $@

$(TESTS_DIR)/parser/test_operators: $(LIBFT_A) $(TESTS_DIR)/parser/test_operators.c $(PARSER_SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) $(TESTS_DIR)/parser/test_operators.c $(PARSER_SRCS) -L$(LIBFT_DIR) -lft -o $@

$(TESTS_DIR)/parser/test_heredoc: $(LIBFT_A) $(TESTS_DIR)/parser/test_heredoc.c $(PARSER_SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) $(TESTS_DIR)/parser/test_heredoc.c $(PARSER_SRCS) -L$(LIBFT_DIR) -lft -o $@

testclean:
	@$(RM) $(TEST_OBJS) $(TEST_NAME)
	@echo "Cleaned test objects and binary."

.PHONY: all clean fclean re localclean test testclean
