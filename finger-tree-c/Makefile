
ftree.o:
	gcc -Wall -Werror -c ftree.c -o ftree.o

check: tests
	./tests

tests: ftree.o
	gcc -Wall -Werror tests.c ftree.o -o ./tests

clean:
	rm -rf tests ftree.o

