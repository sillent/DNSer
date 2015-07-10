/* 
 * File:   main.h
 * Author: santa
 *
 * Created on 9 июля 2015 г., 17:41
 */

#ifndef MAIN_H
#define	MAIN_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/udp.h>
#include <net/if.h>

#include <errno.h>

#include <pcap/pcap.h>


int open_capture();


#ifdef	__cplusplus
extern "C" {
#endif



#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

