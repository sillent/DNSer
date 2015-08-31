#include "worker.h"

void addToList(void *arg);
void removeFromList(void *arg);

static int counter=0;
uint64_t dnsIncoming;
uint64_t dnsOutgoing;
#define THREADNUM 11

pthread_mutex_t mutexsum;
pthread_mutexattr_t mutexattr;

struct timeval tvReq;
void threadWorker(void *arg) {
  
  pcap_packet_t *ar=(pcap_packet_t *)arg;
  tvReq=ar->header.ts;
  ethernet=(struct sniff_ethernet *)(ar->data);
  ip=(struct sniff_ip *)(ar->data+SIZE_ETHERNET);
  size_ip=IP_HL(ip)*4;
  switch(ip->ip_p) {
    case 6: {
//      struct sniff_tcp *tcp=(struct sniff_tcp *)(ar->data+SIZE_ETHERNET+size_ip);
      void *tcp=(void *)(ar->data+SIZE_ETHERNET+size_ip);
      tcpWorker(tcp);
      break;
    }
    case 17: {
//      struct sniff_udp *udp=(struct sniff_udp *)(ar->data+SIZE_ETHERNET+size_ip);
      void *udp=(void *)(ar->data+SIZE_ETHERNET+size_ip);
      udpWorker(udp);
      break;
    }
    default:
      break;
  }
  free(arg); // free memory allocated in packet.c 
}

void tcpWorker(void *tcp) {
  struct sniff_tcp *tcp_t=(struct sniff_tcp *)tcp;
  size_tcp=TH_OFF(tcp_t)*4;

  // check PSH+ACK flags set
  if (tcp_t->th_flags == (TH_ACK+TH_PUSH)) {
    struct sniff_dns_header_tcp_t *dns_header=(struct sniff_dns_header_tcp_t *)(tcp+size_tcp);    
    short qr=N2Hs(dns_header->normal_dns_header.flags) >> 15;   /* Querie or Response */
    if (qr == DNS_RESPONSE) {
      dnsOutgoing++;
    } 
    if (qr==DNS_QUERIE) {
      dnsIncoming++;
    }
  }
}
void udpWorker(void *udp) {
  struct sniff_udp *udp_t=(struct sniff_udp *)udp;
  struct sniff_dns_header_t *dns_header=(struct sniff_dns_header_t *)(udp+SIZE_UDP);
  short qr=N2Hs(dns_header->flags)>>15;     /* Querie or Response */
  if (qr==DNS_RESPONSE) {
    dnsOutgoing++;
  }
  if (qr==DNS_QUERIE) {
    dnsIncoming++;
  }
}

void requestResponse(void* arg,int type) {
  // type: 0 - REQ, 1 - RES
  if (type==0) {
    addToList(arg);
  }
  if (type==1) {
    removeFromList(arg);
  }
}

void addToList(void *arg) {
  
}

void removeFromList(void *arg) {
  
}