/* 
 * File:   worker.h
 * Author: santa
 *
 * Created on 21 июля 2015 г., 10:35
 */

#include "main.h"
typedef struct {
  struct pcap_pkthdr header;
  u_char data[];
} pcap_packet_t;
void doworker(pcap_packet_t *packet_t);
void *threadWorker(void *arg);

