# Makefile

voronoi1: main.o readInputData.o
	gcc -Wall -o voronoi1 main.o readInputData.o

main.o: main.c main.h readInputData.h dcel.h
	gcc -Wall -o main.o main.c -c

readInputData.o : readInputData.c readInputData.h
	gcc -Wall -o readInputData.o readInputData.c -c

#dcel.o: dcel.c dcel.h
#	gcc -Wall -o dcel.o dcel.c -c

# cleaning output files
clean: main
	rm *.o main