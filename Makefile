# Makefile

voronoi1: voronoi1.o readInputData.o dcelOps.o watchtowerOps.o
	gcc -Wall -o voronoi1 voronoi1.o readInputData.o dcelOps.o watchtowerOps.o

voronoi1.o: voronoi1.c readInputData.h dcelOps.h watchtowerOps.h
	gcc -Wall -o voronoi1.o voronoi1.c -c

readInputData.o : readInputData.c readInputData.h
	gcc -Wall -o readInputData.o readInputData.c -c

dcelOps.o: dcelOps.c dcelOps.h
	gcc -Wall -o dcelOps.o dcelOps.c -c

watchtowerOps.o: watchtowerOps.c watchtowerOps.h
	gcc -Wall -o watchtowerOps.o watchtowerOps.c -c

# cleaning output files
clean: voronoi1
	rm *.o voronoi1