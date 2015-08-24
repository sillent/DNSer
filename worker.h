/* 
 * File:   worker.h
 * Author: santa
 *
 * Created on 21 июля 2015 г., 10:35
 */

#include "main.h"
#include "packet.h"
#include <netinet/ip.h>
#include <arpa/inet.h>

extern uint64_t dnsIncoming;
extern uint64_t dnsOutgoing;
extern uint64_t ps_recv;
extern uint64_t ps_drop;
extern uint64_t ps_ifdrop;
extern pthread_mutex_t mutexsum;
extern pthread_mutexattr_t mutexattr;
typedef struct {
  struct pcap_pkthdr header;
  u_char data[];
} pcap_packet_t;
void *doworker(void *packet_t);
void threadWorker(void *arg);

void tcpWorker(void *arg);
void udpWorker(void *arg);

