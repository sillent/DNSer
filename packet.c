//#include "main.h"
#include <netinet/udp.h>
#include "main.h"
#include "worker.h"

void packet_sniff(u_char* args, const struct pcap_pkthdr* header, const u_char* packet) {
  pcap_packet_t *packet_t=malloc(sizeof(struct pcap_pkthdr)+header->caplen);
  packet_t->header=*header;
  memcpy(packet_t->data,packet,header->caplen);
  doworker(packet_t);
  free(packet_t);
}
