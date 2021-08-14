# Makefile

main: main.o readCSVData.o
	gcc -Wall -o main main.o readCSVData.o

main.o: main.c main.h readCSVData.h dcel.h
	gcc -Wall -o main.o main.c -c

readCSVData.o : readCSVData.c readCSVData.h
	gcc -Wall -o readCSVData.o readCSVData.c -c

#dcel.o: dcel.c dcel.h
#	gcc -Wall -o dcel.o dcel.c -c

# cleaning output files
clean: main
	rm *.o main