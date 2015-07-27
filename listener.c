#include <sys/types.h>

#include "listener.h"
static int sockfd;
uint64_t dnsIncoming;
uint64_t dnsOutgoing;
void * listener() {
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
//        int recl=recvfrom(sockfd,msg,sizeof(struct mesg_t),0,(struct sockaddr *)&client_addr,&sendsize);
//        if (recl >= 0) {
//            grepp(*msg,client_addr);
//        }
//      printf("resp:%lu resp:%lu\n",dnsOutgoing);
      grepp(*msg,client_addr);
      sleep(1);
    }
    return (void*)1;
}
void grepp(struct mesg_t data,struct sockaddr_in respondto) {
    struct mesg_t d=data;
    char msg[255];
    snprintf(msg,255,"resp:%lu resp:%lu\n",dnsOutgoing,dnsOutgoing);
    printf("%s",msg);
    printf("inc address:%p\n",&dnsIncoming);
    printf("out address:%p\n",&dnsOutgoing);
    printf("inc:%p out:%p\n", &dnsIncoming,&dnsOutgoing);
    printf("inc:%lu out:%lu\n", dnsIncoming,dnsOutgoing);
//    if (strcmp(d.mes,"get")==0) {
//      sendmsgto(respondto,dnsi)
//    }
    
}
int sendmsgto(struct sockaddr_in to, u_char *mesage) {
//    long long t=htobe64(message);
  uint64_t t;
    int rs=sendto(sockfd,&t,sizeof(t),0,(struct sockaddr *)&to,sizeof(to));
//    printf("send socket: %d\n",rs);
    return rs;
}