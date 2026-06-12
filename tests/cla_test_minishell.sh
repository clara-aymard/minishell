#!/bin/bash

MINISHELL="./minishell"
TMP_DIR="./tmp_minishell_tests"

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

	# Supprime les couleurs ANSI
	sed -i -r 's/\x1B\[[0-9;]*[mK]//g' "$file"

	# Supprime les prompts minishell
	sed -i '/^mini\$ /d' "$file"
	sed -i '/^mini\$$/d' "$file"

	# Supprime le "exit" affiché à la fermeture
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
		echo -e "${YELLOW}Commande:${RESET} $cmd"

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

# BASE
run_test "echo hello" "echo simple"
run_test "pwd" "pwd"
run_test "ls" "ls"
run_test "/bin/echo ok" "chemin absolu"
run_test "commande_inexistante" "commande inexistante"

# EXIT STATUS
run_test $'false\necho $?' "status false"
run_test $'true\necho $?' "status true"
run_test $'ls fichier_inexistant\necho $?' "status erreur ls"
run_test $'commande_inexistante\necho $?' "status command not found"

# REDIRECTIONS
run_test $'echo hi > tmp_minishell_tests/out\ncat tmp_minishell_tests/out' "redirection >"
run_test $'echo hi > tmp_minishell_tests/out\necho there >> tmp_minishell_tests/out\ncat tmp_minishell_tests/out' "redirection >>"
run_test "cat < fichier_inexistant" "redirection input inexistante"
run_test $'echo test > tmp_minishell_tests/a > tmp_minishell_tests/b\ncat tmp_minishell_tests/a\ncat tmp_minishell_tests/b' "double redirection output"
run_test $'echo hello > tmp_minishell_tests/out\ncat < tmp_minishell_tests/out' "redirection input valide"
run_test $'echo one > tmp_minishell_tests/out\necho two > tmp_minishell_tests/out\ncat tmp_minishell_tests/out' "redirection overwrite"
run_test "echo hello > /tmp/minishell_abs_test" "redirection chemin absolu"

# PIPES
run_test "echo hello | cat" "pipe simple"
run_test "echo hello | grep h" "pipe grep"
run_test "echo hello | grep z" "pipe grep no match"
run_test "cat /dev/null | cat | wc -l" "pipe triple"
run_test "echo hello | grep h | wc -c" "pipe triple avec output"
run_test "printf 'a\nb\nc\n' | grep b" "pipe avec plusieurs lignes"
run_test "echo hello | wc -w" "pipe wc words"
run_test "echo hello | cat | cat | cat" "pipe long cat"
run_test "cat < nofile | echo ok" "redirection fail dans pipe"
run_test "echo test > tmp_minishell_tests/a | cat" "redirection avant pipe"

# BUILTINS
run_test $'export A=42\necho $A' "export"
run_test $'export A=42\nunset A\necho $A' "unset"
run_test $'cd /\npwd' "cd absolu"
run_test $'cd /tmp\ncd -\npwd' "cd -"
run_test $'export EMPTY=\necho "$EMPTY"' "export variable vide"
run_test $'export A=hello\nexport B=world\necho "$A $B"' "plusieurs exports"
run_test $'pwd\ncd /\npwd\ncd /tmp\npwd' "plusieurs cd"

# QUOTES / EXPANSIONS EN PLUS
run_test 'echo "$HOME/test"' "expansion collée texte"
run_test 'echo abc"$HOME"def' "expansion au milieu"
run_test "echo ''a''" "quotes vides collées"
run_test 'echo ""a""' "double quotes vides collées"
run_test 'echo "$UNDEFINED"' "variable inexistante double quotes"
run_test "echo '$UNDEFINED'" "variable inexistante single quotes"
run_test 'echo "$?"' "status dans double quotes"

# CAS TORDUS
run_test "echo \"\"" "echo empty double quotes"
run_test "echo ''" "echo empty single quotes"
run_test "echo    hello      world" "espaces multiples"
run_test "echo hello      " "espaces fin de ligne"
run_test "      echo hello" "espaces début de ligne"
run_test "echo hello |" "syntax pipe fin"
run_test "| echo hello" "syntax pipe début"
run_test "echo >" "syntax redirection seule"
run_test "cat <" "syntax input seule"

echo
echo "Résultat: $passed/$total tests OK"

rm -rf "$TMP_DIR"

# chmod +x test_minishell.sh
# ./test_minishell.sh

# chmod +x tests/test_minishell.sh
# chmod +x tests/valgrind_minishell.sh
# git add tests/test_minishell.sh tests/valgrind_minishell.sh
# git commit -m "Make test scripts executable"
