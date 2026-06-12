#!/bin/bash

MINISHELL="./minishell"
TMP_DIR="./tmp_minishell_tests"

KEEP_TMP=${KEEP_TMP:-0}

mkdir -p "$TMP_DIR"

GREEN="\033[32m"
RED="\033[31m"
YELLOW="\033[33m"
RESET="\033[0m"

total=0
passed=0

clean_output()
{
	file="$1"

	sed -i -r 's/\x1B\[[0-9;]*[mK]//g' "$file"

	sed -i '/^mini\$ /d' "$file"
	sed -i '/^mini\$$/d' "$file"

	sed -i '/^exit$/d' "$file"
}

run_test()
{
	cmd="$1"
	name="$2"

	total=$((total + 1))

	bash_out="$TMP_DIR/bash_out"
	bash_err="$TMP_DIR/bash_err"
	mini_out="$TMP_DIR/mini_out"
	mini_err="$TMP_DIR/mini_err"

	bash -c "$cmd" > "$bash_out" 2> "$bash_err"
	bash_status=$?

	printf "%s\n" "$cmd" | "$MINISHELL" > "$mini_out" 2> "$mini_err"
	mini_status=$?

	clean_output "$mini_out"
	clean_output "$mini_err"

	if diff -q "$bash_out" "$mini_out" >/dev/null \
		&& diff -q "$bash_err" "$mini_err" >/dev/null \
		&& [ "$bash_status" -eq "$mini_status" ]; then
		echo -e "${GREEN}[OK]${RESET} $name"
		passed=$((passed + 1))
	else
		echo -e "${RED}[KO]${RESET} $name"
		echo -e "${YELLOW}Command:${RESET} $cmd"

		echo "--- stdout bash ---"
		cat "$bash_out"

		echo "--- stdout minishell ---"
		cat "$mini_out"

		echo "--- stderr bash ---"
		cat "$bash_err"

		echo "--- stderr minishell ---"
		cat "$mini_err"

		echo "--- status ---"
		echo "bash:      $bash_status"
		echo "minishell: $mini_status"
		echo
	fi
}

run_test "echo hello" "simple echo"
run_test "pwd" "pwd"
run_test "ls" "ls"
run_test "/bin/echo ok" "absolute path"
run_test "non_existent_command" "command not found"

run_test $'false\necho $?' "false exit status"
run_test $'true\necho $?' "true exit status"
run_test $'ls non_existent_file\necho $?' "ls error status"
run_test $'non_existent_command\necho $?' "command not found status"

run_test $'echo hi > tmp_minishell_tests/out\ncat tmp_minishell_tests/out' "output redirection"
run_test $'echo hi > tmp_minishell_tests/out\necho there >> tmp_minishell_tests/out\ncat tmp_minishell_tests/out' "append redirection"
run_test "cat < non_existent_file" "missing input redirection"
run_test $'echo test > tmp_minishell_tests/a > tmp_minishell_tests/b\ncat tmp_minishell_tests/a\ncat tmp_minishell_tests/b' "multiple output redirections"
run_test $'echo hello > tmp_minishell_tests/out\ncat < tmp_minishell_tests/out' "valid input redirection"
run_test $'echo one > tmp_minishell_tests/out\necho two > tmp_minishell_tests/out\ncat tmp_minishell_tests/out' "overwrite redirection"
run_test "echo hello > /tmp/minishell_abs_test" "absolute path redirection"

run_test "echo hello | cat" "simple pipe"
run_test "echo hello | grep h" "pipe with grep"
run_test "echo hello | grep z" "pipe with no grep match"
run_test "cat /dev/null | cat | wc -l" "three-stage pipeline"
run_test "echo hello | grep h | wc -c" "pipeline with output count"
run_test "printf 'a\nb\nc\n' | grep b" "multi-line pipe"
run_test "echo hello | wc -w" "pipe with wc"
run_test "echo hello | cat | cat | cat" "long pipe"
run_test "cat < non_existent_file | echo ok" "failed redirection in pipeline"
run_test "echo test > tmp_minishell_tests/a | cat" "redirection before pipe"

run_test $'export A=42\necho $A' "export variable"
run_test $'export A=42\nunset A\necho $A' "unset variable"
run_test $'cd /\npwd' "absolute cd"
run_test $'cd /tmp\ncd -\npwd' "cd previous directory"
run_test $'export EMPTY=\necho "$EMPTY"' "empty exported variable"
run_test $'export A=hello\nexport B=world\necho "$A $B"' "multiple exported variables"
run_test $'pwd\ncd /\npwd\ncd /tmp\npwd' "multiple cd commands"

run_test 'echo "$HOME/test"' "expansion followed by text"
run_test 'echo abc"$HOME"def' "expansion in the middle of text"
run_test "echo ''a''" "empty single quotes joined with text"
run_test 'echo ""a""' "empty double quotes joined with text"
run_test 'echo "$UNDEFINED"' "undefined variable in double quotes"
run_test "echo '$UNDEFINED'" "undefined variable in single quotes"
run_test 'echo "$?"' "exit status in double quotes"

run_test "echo \"\"" "empty double quotes"
run_test "echo ''" "empty single quotes"
run_test "echo    hello      world" "multiple spaces"
run_test "echo hello      " "trailing spaces"
run_test "      echo hello" "leading spaces"
run_test "echo hello |" "syntax error: trailing pipe"
run_test "| echo hello" "syntax error: leading pipe"
run_test "echo >" "syntax error: missing output file"
run_test "cat <" "syntax error: missing input file"

echo
echo "Result: $passed/$total tests passed"

rm -f /tmp/minishell_abs_test

if [ "$KEEP_TMP" -eq 0 ]; then
	rm -rf "$TMP_DIR"
else
	echo
	echo "Temporary test files stored in: $TMP_DIR"
fi