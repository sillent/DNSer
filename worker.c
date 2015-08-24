

#include "worker.h"

static int counter=0;
uint64_t dnsIncoming;
uint64_t dnsOutgoing;
#define THREADNUM 11

pthread_mutex_t mutexsum;
pthread_mutexattr_t mutexattr;
void *doworker(void *packet) {
  /* thread created int packet.c 
  pthread_t thread[THREADNUM];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
  pthread_mutex_init(&mutexsum,NULL);
//  printf("thread go %d\n", counter);
  if (counter>THREADNUM) {
    counter=0;
  } else {
    counter++;
  }
//  printf("create threadid :%d and threadaddress:%p\n",counter,&thread[counter]);
  pthread_create(&thread[counter],&attr,threadWorker,(void*)packet);
//  pthread_join(thread[counter],NULL);
  pthread_attr_destroy(&attr);
   */
//  pthread_mutex_init(&mutexsum,NULL);
  threadWorker(packet);
//  pthread_mutex_destroy(&mutexsum);
//  pthread_exit(NULL);
}

void threadWorker(void *arg) {
  
  pcap_packet_t *ar=(pcap_packet_t *)arg;
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
//    printf("mutex: %p\n",&mutexsum);
    int lc=pthread_mutex_lock(&mutexsum);
    if (lc!=0) {
      fprintf(stderr, "Error lock: lc=%d\n",lc);
    }
    if (qr == DNS_RESPONSE) {
      dnsOutgoing++;
    } 
    if (qr==DNS_QUERIE) {
      dnsIncoming++;
    }
    int ul=pthread_mutex_unlock(&mutexsum);
    if (ul!=0) {
      fprintf(stderr,"Error unlock: ul=%d\n",ul);
    }
  }
}
void udpWorker(void *udp) {
  struct sniff_udp *udp_t=(struct sniff_udp *)udp;
  struct sniff_dns_header_t *dns_header=(struct sniff_dns_header_t *)(udp+SIZE_UDP);
  short qr=N2Hs(dns_header->flags)>>15;     /* Querie or Response */
//    printf("mutex: %p\n",&mutexsum);
  int lc=pthread_mutex_lock(&mutexsum);
  if (lc!=0) {
    fprintf(stderr, "Error lock: lc=%d\n",lc);
  }
  if (qr==DNS_RESPONSE) {
    dnsOutgoing++;
  }
  if (qr==DNS_QUERIE) {
    dnsIncoming++;
  }
  int ul=pthread_mutex_unlock(&mutexsum);
  if (ul!=0) {
    fprintf(stderr,"Error unlock: ul=%d\n",ul); 
  }
}
