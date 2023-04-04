test: tests/manager_test.c 
	gcc -Wall -Wextra -pedantic -std=c99 -o test.exe tests/manager_test.c src/manager.c include/dslib.c

cli: runtime.c
	gcc -Wall -Wextra -pedantic -std=c99 -o manager.exe runtime.c src/manager.c include/dslib.c

all: test cli

.PHONY: clean

clean:
	rm manager.exe
	rm test.exe