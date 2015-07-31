#include <sys/types.h>

#include "main.h"
#define BUFERLEN 4194304
#define SNAPLEN 1500
pcap_t *handle;
void start_sniff(char* devName) {
//  pcap_t *handle;  Go to global

  struct bpf_program filter_program;
  bpf_u_int32 netmask;
  bpf_u_int32 network;

  struct pcap_pkthdr packet_header_t;
  const u_char *packet;
  
  int lookup_net_retval=pcap_lookupnet(devName,&network,&netmask,errbuf);
  if (lookup_net_retval==-1) {
    fprintf(stderr,"Faild lookupnet\n");
    netmask=0;
    network=0;
  }
  
  handle=pcap_create(devName,errbuf);
  if (pcap_set_buffer_size(handle,BUFERLEN*4)!=0) {
    fprintf(stderr,"FAIL.Can't set buffer size\n");
    exit(1);
  }
  if (pcap_set_promisc(handle,0)!=0) {
    fprintf(stderr,"FAIL.Can't set no promisc\n");
    exit(EXIT_FAILURE);
  }
  if (pcap_set_snaplen(handle,SNAPLEN)!=0) {
    fprintf(stderr,"FAIL.Can't set snaplen\n");
    exit(EXIT_FAILURE);
  }
  if (pcap_set_timeout(handle,0)!=0) {
    fprintf(stderr,"FAIL.Can't set timeout\n");
    exit(EXIT_FAILURE);
  }
//  handle=pcap_open_live(devName,1500,0,0,errbuf);
//  if (handle==NULL) {
//    fprintf(stderr,"FAIL. Can't start sniffing on dev %s\n",devName);
//    exit(1);
//  }
  if (pcap_activate(handle)!=0) {
    fprintf(stderr, "FAIL.Can't activate handle\n");
    exit(EXIT_FAILURE);
  }
  if (pcap_compile(handle,&filter_program,"port 53",0,netmask)==-1) {
    fprintf(stderr,"FAIL. Can't compile sniffing filter rule\n");
    exit(1);
  }
  if (pcap_setfilter(handle,&filter_program)==-1) {
    fprintf(stderr,"FAIL. Can't set filter\n");
    exit(1);
  }
  
  pcap_loop(handle,0,packet_sniff,NULL);
}