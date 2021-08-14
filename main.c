/*
 * Main program for COMP20003 Assignment 1
 */

#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "readCSVData.h"

int main() {

    FILE *CSVData;
    char **watchtowersStringsArray
    watchtowerData *watchtowersStructsArray;

    CSVData = fopen(argv[0], r);
    watchtowersStringsArray = readCSVData(CSVData)


    printf("program finished");
    return 0;
}

