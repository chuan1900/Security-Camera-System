all:a3-ece650 a2-ece650 rgen
a3-ece650:pipe0.o
	gcc -o a3-ece650 pipe0.o
a2-ece650:a2-ece650.o
	gcc -o a2-ece650 a2-ece650.o
rgen:rgen.o
	gcc -o rgen rgen.o
pipe0.o:pipe0.c
	gcc -c pipe0.c
a2-ece650.o:a2-ece650.c
	gcc -c a2-ece650.c
rgen.o:rgen.c
	gcc -c rgen.c
clean:
	rm -rf *.o a3-ece650 a2-ece650 rgen