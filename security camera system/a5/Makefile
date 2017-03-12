all:a5-ece650
a5-ece650:main.o
	g++ main.o -pthread -L. -lsat -o a5-ece650 
main.o:main.c SAT.h
	g++ -c main.c                    
clean:
	rm -rf main.o a5-ece650
