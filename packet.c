//#include "main.h"
#include <netinet/udp.h>
#include <asm-generic/errno-base.h>
#include "main.h"
#include "worker.h"

void packet_sniff(u_char* args, const struct pcap_pkthdr* header, const u_char* packet) {
  pcap_packet_t *packet_t=malloc(sizeof(struct pcap_pkthdr)+header->caplen);
  packet_t->header=*header;
  memcpy(packet_t->data,packet,header->caplen);
  threadWorker((void*)packet_t);
}

//void *startDoWorker(void *arg) {
//  
//  free(arg);
//   printf("doworker finishing\n");
//   pthread_exit(NULL);
//}
