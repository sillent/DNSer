/* 
 * File:   main.c
 * Author: santa
 *
 * Created on 9 июля 2015 г., 17:38
 */

#include "main.h"

/*
 * 
 */
int main(int argc, char** argv) {
    printf("arg: %s\n", argv[1]);
    char *devName=argv[1];
    if (find_device(devName)==DEVFND) {
      start_sniff(devName);
    }
    return (EXIT_SUCCESS);
}

