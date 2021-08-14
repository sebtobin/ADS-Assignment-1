/*
 * Main program for COMP20003 Assignment 1
 */

#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "readCSVData.h"

int main(int argc, char *argv[]) {

    FILE *CSVData;
    char **watchtowerStringArray;
    watchtowerData *watchtowerStructArray;

    CSVData = fopen(argv[1], "r");
    watchtowerStringArray = readCSVData(CSVData, watchtowerStringArray);
    //watchtowerStructArray =

    printf("program finished");
    return 0;
}



