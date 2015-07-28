#include <sys/types.h>

#include "main.h"

void start_sniff(char* devName) {
  pcap_t *handle;
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
  
  handle=pcap_open_live(devName,BUFSIZ,0,0,errbuf);
  if (handle==NULL) {
    fprintf(stderr,"FAIL. Can't start sniffing on dev %s\n",devName);
    exit(1);
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