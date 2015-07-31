/* 
 * File:   worker.h
 * Author: santa
 *
 * Created on 21 июля 2015 г., 10:35
 */

#include "main.h"
#include "packet.h"

extern uint64_t dnsIncoming;
extern uint64_t dnsOutgoing;
extern uint64_t ps_recv;
extern uint64_t ps_drop;
extern uint64_t ps_ifdrop;
pthread_mutex_t mutexsum;
pthread_mutexattr_t mutexattr;

typedef struct {
  struct pcap_pkthdr header;
  u_char data[];
} pcap_packet_t;
void doworker(pcap_packet_t *packet_t);
void *threadWorker(void *arg);

void tcpWorker(void *arg);
void udpWorker(void *arg);

