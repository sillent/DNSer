//#include "main.h"
#include <netinet/udp.h>
#include "main.h"
#include "worker.h"
void *startDoWorker(void *arg);
void packet_sniff(u_char* args, const struct pcap_pkthdr* header, const u_char* packet) {
  pcap_packet_t *packet_t=malloc(sizeof(struct pcap_pkthdr)+header->caplen);
  packet_t->header=*header;
  memcpy(packet_t->data,packet,header->caplen);
  pthread_t thread;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
  if (pthread_create(&thread,&attr,startDoWorker,(void*)packet_t)!=0) {
    fprintf(stderr,"Can't create thread in packetSniff\n");
  }
//  doworker(packet_t);
  printf("%p\n",&thread);
  pthread_attr_destroy(&attr);
  free(packet_t);
}

void *startDoWorker(void *arg) {
  doworker((pcap_packet_t *)arg);
//  free(arg);
}