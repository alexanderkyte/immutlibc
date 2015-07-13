
ftree.o:
	gcc -c ftree.c -o ftree.o

check: tests
	./tests

tests: ftree.o
	gcc tests.c ftree.o -o ./tests

clean:
	rm -rf tests ftree.o

