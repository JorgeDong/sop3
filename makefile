all: spi posix clone

spi: serialPI.c
	gcc -o spi serialPI.c -lm

posix: posixPI.c
	gcc -o ppi posixPI.c -lm -lpthread

clone: clonePI.c
	gcc -o cpi clonePI.c -lm 
