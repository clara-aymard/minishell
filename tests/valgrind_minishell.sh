#!/bin/bash

MINISHELL="./minishell"
TMP_DIR="./tmp_valgrind_tests"
INPUT="$TMP_DIR/minishell_valgrind_input"
LOG="$TMP_DIR/valgrind_full.log"

mkdir -p "$TMP_DIR"

cat > "$INPUT" <<'EOF'
ls
pwd
echo hello
non_existent_command
/bin/ls
./nofile

echo hi > tmp_valgrind_tests/out
cat < tmp_valgrind_tests/out
cat < nofile
echo hi > /root/nope
echo hi > tmp_valgrind_tests/a > tmp_valgrind_tests/b
echo hi >> tmp_valgrind_tests/a >> tmp_valgrind_tests/b

export A=42
echo $A
unset A
echo $A
export EMPTY=
echo "$EMPTY"

cd /
pwd
cd /tmp
cd -
pwd

env

echo "$HOME"
echo '$HOME'
echo "$?"
echo "$UNDEFINED"
echo "'$HOME'"
echo '"$HOME"'

ls | cat
ls | grep Makefile
cat < nofile | echo ok
echo ok | cat < nofile
cat /dev/null | cat | wc -l
echo hello | cat | cat | cat | wc -c

exit
EOF

valgrind \
	--leak-check=full \
	--show-leak-kinds=all \
	--track-fds=yes \
	--track-origins=yes \
	--log-file="$LOG" \
	"$MINISHELL" < "$INPUT"

echo
echo "Valgrind report written to: $LOG"
echo

echo "===== LEAK SUMMARY ====="
grep -A8 "LEAK SUMMARY" "$LOG"

echo
echo "===== FILE DESCRIPTORS ====="
grep -A20 "FILE DESCRIPTORS" "$LOG"

echo
echo "===== ERROR SUMMARY ====="
grep "ERROR SUMMARY" "$LOG"

echo
echo "Temporary test files stored in: $TMP_DIR"