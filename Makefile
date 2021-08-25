# Makefile

voronoi1: main.o readInputData.o dcelOps.o watchtowerOps.o
	gcc -Wall -o voronoi1 main.o readInputData.o dcelOps.o watchtowerOps.o

main.o: main.c readInputData.h dcelOps.h watchtowerOps.h
	gcc -Wall -o main.o main.c -c

readInputData.o : readInputData.c readInputData.h
	gcc -Wall -o readInputData.o readInputData.c -c

dcelOps.o: dcelOps.c dcelOps.h
	gcc -Wall -o dcelOps.o dcelOps.c -c

watchtowerOps.o: watchtowerOps.c watchtowerOps.h
	gcc -Wall -o watchtowerOps.o watchtowerOps.c -c

# cleaning output files
clean: vorono1
	rm *.o voronoi1