test: tests/manager_test.c 
	gcc -Wall -Wextra -pedantic -std=c99 -o test.exe tests/manager_test.c src/manager.c -L/include -lds

cli: runtime.c
	gcc -Wall -Wextra -pedantic -std=c99 -o manager.exe runtime.c src/manager.c -L/include -B/include

all: test cli

.PHONY: clean

clean:
	rm manager.exe
	rm test.exe