
#include <netinet/ip.h>
#include <arpa/inet.h>

#include "worker.h"

static int counter=0;

void doworker(pcap_packet_t *packet) {
  pthread_t thread[4];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
  printf("thread go %d\n", counter);
  if (counter>3) {
    counter=0;
  } else {
    counter++;
  }
  pthread_create(&thread[counter],&attr,threadWorker,(void*)packet);
//  pthread_join(thread,NULL);
}

void *threadWorker(void *arg) {
  pcap_packet_t *ar=(pcap_packet_t *)arg;
  ethernet=(struct sniff_ethernet *)(ar->data);
  ip=(struct sniff_ip *)(ar->data+SIZE_ETHERNET);
  size_ip=IP_HL(ip)*4;
  switch(ip->ip_p) {
    case 6: {
      struct sniff_tcp *tcp=(struct sniff_tcp *)(ar->data+SIZE_ETHERNET+size_ip);
      tcpWorker(tcp);
      break;
    }
    case 17: {
      struct sniff_udp *udp=(struct sniff_udp *)(ar->data+SIZE_ETHERNET+size_ip);
      udpWorker(udp);
      break;
    }
    default:
      break;
  }
}

void tcpWorker(struct sniff_tcp *tcp) {
}
void udpWorker(struct sniff_udp *udp) {
  printf("src: %u dst: %u\n",ntohs(udp->src_port),ntohs(udp->dst_port));
}