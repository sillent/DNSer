#include <sys/types.h>

#include "listener.h"
#include <signal.h>

void sighandler(int signum);

static int sockfd;
uint64_t dnsIncoming;
uint64_t dnsOutgoing;
pcap_t *handle;
uint64_t ps_recv;
uint64_t ps_drop;
uint64_t ps_ifdrop;

void * listener() {
  signal(SIGINT,sighandler);
  
  sockfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (sockfd == -1) {
        fprintf(stderr,"Cannot create socket on listener()\n");
        exit(EXIT_FAILURE);
    }
    struct mesg_t *msg=malloc(sizeof(struct mesg_t));
    memset(msg,0,sizeof(struct mesg_t));
    struct sockaddr_in serv_addr, client_addr;
//    memset(&serv_addr,0,sizeof(serv_addr));
    memset(&client_addr,0,sizeof(client_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr("0.0.0.0");
    serv_addr.sin_port=htons(SERVER_PORT);
    
    int rb=bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    if (rb<0) {
        fprintf(stderr,"Cannot bind socket on listener()\n");
        exit(EXIT_FAILURE);
    }

    socklen_t sendsize=sizeof(client_addr);
    while (1) {
        int recl=recvfrom(sockfd,msg,sizeof(struct mesg_t),0,(struct sockaddr *)&client_addr,&sendsize);
        if (recl >= 0) {
            grepp(*msg,client_addr);
        }
    }
    return (void*)1;
}
void grepp(struct mesg_t data,struct sockaddr_in respondto) {
  doStat();
  if (strncmp(data.mes,"req",3)==0) {
    sendmsgto(respondto,dnsIncoming);
  } 
  if (strncmp(data.mes,"res",3)==0) {
    sendmsgto(respondto,dnsOutgoing);
  }
  if (strncmp(data.mes,"st_ifdrop",9)==0) {
    sendmsgto(respondto,ps_ifdrop);
  }
  if (strncmp(data.mes,"st_drop",7)==0) {
      sendmsgto(respondto,ps_drop);
  }
  if (strncmp(data.mes,"st_recv",7)==0) {
      sendmsgto(respondto,ps_recv);
  }
}
int sendmsgto(struct sockaddr_in to, uint64_t mesage) {
  uint64_t t=htobe64(mesage);
  int rs=sendto(sockfd,&t,sizeof(t),0,(struct sockaddr *)&to,sizeof(to));
//    printf("send socket: %d\n",rs);
  return rs;
}

void doStat() {
  struct pcap_stat stat;
  memset(&stat,0,sizeof(struct pcap_stat));
  if (pcap_stats(handle,&stat)==-1) {
    printf("error stat\n");
  }
  ps_drop=stat.ps_drop;
  ps_ifdrop=stat.ps_ifdrop;
  ps_recv=stat.ps_recv;
}

void sighandler(int signum) {
  if (signum=SIGINT) {
    printf("received SIGINT. EXITED\n");
    exit(EXIT_FAILURE);
  }
}