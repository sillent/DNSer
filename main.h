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
#include <string.h>
#include <errno.h>

#include <pcap.h>

#define DEVFND 1
#define DEVSNF 4
#define DEVNF 5

char errbuf[256];

void error(int code);
int find_device(char *devName);  // search sepcific interface
pcap_if_t *find_devices();         // search all available interfaces
void start_sniff(char *devName);    // capturing.c

void packet_sniff(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

#ifdef	__cplusplus
extern "C" {
#endif



#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

