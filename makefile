all: spi posix

spi: serialPI.c
	gcc -o spi serialPI.c -lm

posix: posixPI.c
	gcc -o ppi posixPI.c -lm -lpthread
