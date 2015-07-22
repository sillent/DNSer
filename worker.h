/* 
 * File:   worker.h
 * Author: santa
 *
 * Created on 21 июля 2015 г., 10:35
 */

#include "main.h"
#include "packet.h"
typedef struct {
  struct pcap_pkthdr header;
  u_char data[];
} pcap_packet_t;
void doworker(pcap_packet_t *packet_t);
void *threadWorker(void *arg);

void tcpWorker(void *arg);
void udpWorker(void *arg);

