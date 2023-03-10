all: tests/manager_test.c 
	gcc -Wall -Wextra -pedantic -std=c99 -o manager.exe tests/manager_test.c src/manager.c include/dslib.c

.PHONY: clean

clean:
	rm manager.exe