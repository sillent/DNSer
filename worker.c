
#include <netinet/ip.h>
#include <arpa/inet.h>

#include "worker.h"
#include "packet.h"

void doworker(pcap_packet_t *packet) {
  pthread_t thread;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
  printf("thread go\n");
  pthread_create(&thread,&attr,threadWorker,(void*)packet);
}

void *threadWorker(void *arg) {
  printf("worker");
}