FLAGS = -Wall -Wextra -Werror -std=c11
FILE := test1.txt
FILE2 := test2.txt
FILE3 := test3.txt
all: s21_grep tests

s21_grep: Makefile s21_grep.h s21_grep.c 
	gcc $(FLAGS) s21_grep.c -o s21_grep

check:
	clang-format -style=Google -n *.c

rebuild:
	make clean
	make all

clean:
	rm -f s21_grep

tests:
	@bash -c 'diff -w <(./s21_grep -v maks $(FILE)) <(grep -v maks $(FILE))'
	@bash -c 'diff -w <(./s21_grep maks $(FILE)) <(grep maks $(FILE))'
	@bash -c 'diff -w <(./s21_grep -o maks $(FILE)) <(grep -o maks $(FILE))'
	@bash -c 'diff -w <(./s21_grep -i maks $(FILE)) <(grep -i maks $(FILE))'
	@bash -c 'diff -w <(./s21_grep -c maks $(FILE)) <(grep -c maks $(FILE))'
	@bash -c 'diff -w <(./s21_grep -l maks $(FILE)) <(grep -l maks $(FILE))'
	@bash -c 'diff -w <(./s21_grep -n maks $(FILE)) <(grep -n maks $(FILE))'
	@bash -c 'diff -w <(./s21_grep -h maks $(FILE)) <(grep -h maks $(FILE))'
	@bash -c 'diff -w <(./s21_grep -f patterns.txt $(FILE)) <(grep -f patterns.txt $(FILE))'
	@bash -c 'diff -w <(./s21_grep -vi maks $(FILE)) <(grep -vi maks $(FILE))' 
	@bash -c 'diff -w <(./s21_grep -co maks $(FILE)) <(grep -co maks $(FILE))' 
	@bash -c 'diff -w <(./s21_grep -lo maks $(FILE)) <(grep -lo maks $(FILE))' 
	@bash -c 'diff -w <(./s21_grep -ni maks $(FILE)) <(grep -ni maks $(FILE))'