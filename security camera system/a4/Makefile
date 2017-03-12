all:a4-ece650
a4-ece650:main.o
	g++ main.o -L. -lsat -o a4-ece650 
main.o:main.c SAT.h
	g++ -c main.c                    
clean:
	rm -rf main.o a4-ece650
