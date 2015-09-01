#include "worker.h"
#define REQUEST 0
#define RESPONS 1

#define TINY 10.000000
#define SMALL 100.000000
#define LITLE 500.000000
#define MIDDLE 800.000000
#define BIG 1600.000000


void addToArray(void *arg);
void removeFromArray(void *arg);
uint64_t differenceTime(struct timeval tvReq, struct timeval tvRes);

uint64_t dnsIncoming;
uint64_t dnsOutgoing;     

reqId_t reqId[65535];
rtt_t    dnsRTT;
//pthread_mutex_t mutexsum;
//pthread_mutexattr_t mutexattr;

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
//  printf("tiny:%lu small:%lu litle:%lu middle:%lu big:%lu huge:%lu\n",dnsRTT.tiny, dnsRTT.litle,dnsRTT.middle,dnsRTT.big,dnsRTT.huge);
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
    requestResponse((void *)dns_header,RESPONS);
  }
  if (qr==DNS_QUERIE) {
    dnsIncoming++;
    requestResponse((void *)dns_header,REQUEST);
  }
}

void requestResponse(void* arg,int type) {
  // type: 0 - REQ, 1 - RES
  if (type==0) {
    addToArray(arg);
//    printf("req:sec: %u usec:%u\n",reqId[((struct sniff_dns_header_t *)arg)->id-1].tv.tv_sec,reqId[((struct sniff_dns_header_t *)arg)->id-1].tv.tv_usec);
  }
  if (type==1) {
//    printf("res:sec: %u usec:%u\n",tvReq.tv_sec, tvReq.tv_usec);
    removeFromArray(arg);
  }
}

void addToArray(void *arg) {
  struct sniff_dns_header_t *head=(struct sniff_dns_header_t *)arg;
  reqId[(head->id)-1].set=1;
  reqId[(head->id)-1].tv=tvReq;  
}

void removeFromArray(void *arg) {
  struct sniff_dns_header_t *head=(struct sniff_dns_header_t *)arg;
  // Diff Time Req-Res
  float time=(tvReq.tv_sec-reqId[(head->id)-1].tv.tv_sec)*1000+(tvReq.tv_usec-reqId[(head->id)-1].tv.tv_usec)/(float)1000;
  if (time < TINY ) {
    dnsRTT.tiny++;
  } else if (time > TINY && time < SMALL ) {
    dnsRTT.small++;
  } else if (time > SMALL && time < LITLE ) {
    dnsRTT.litle++;
  } else if (time > LITLE && time < MIDDLE ) {
    dnsRTT.middle++;
  } else if (time > MIDDLE && time < BIG ) {
    dnsRTT.big++;
  } else if (time > BIG ) {
    dnsRTT.huge++;
  }
//  printf("diffTime%lu\n", diffTime);
  reqId[(head->id)-1].set=0;
  reqId[(head->id)-1].tv.tv_sec=0;
  reqId[(head->id)-1].tv.tv_usec=0;
}

//uint64_t differenceTime(struct timeval tvReq, struct timeval tvRes) {
//  uint64_t req=(tvReq.tv_sec * (uint64_t)1000)+(tvReq.tv_usec/1000);
//  uint64_t res=(tvRes.tv_sec * (uint64_t)1000)+(tvRes.tv_usec/1000);
//  return res-req;
//}