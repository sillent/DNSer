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
    if (argc>=2) {
      size_t argLen=strlen(argv[1]);
      char *devName=malloc(argLen+1);
      strncpy(devName,argv[1],argLen);
      if (find_device(devName)==DEVFND) {
        start_sniff(devName);
      }
    }
    return (EXIT_SUCCESS);
}

