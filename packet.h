/* 
 * File:   packet.h
 * Author: santa
 *
 * Created on 21 июля 2015 г., 12:01
 */
#include <netinet/in.h>
#define ETHER_ADDR_LEN	6

	/* Ethernet header */
struct sniff_ethernet {
  u_char ether_dhost[ETHER_ADDR_LEN]; /* Destination host address */
  u_char ether_shost[ETHER_ADDR_LEN]; /* Source host address */
  u_short ether_type; /* IP? ARP? RARP? etc */
};

	/* IP header */
struct sniff_ip {
  u_char ip_vhl;		/* version << 4 | header length >> 2 */
  u_char ip_tos;		/* type of service */
  u_short ip_len;		/* total length */
  u_short ip_id;		/* identification */
  u_short ip_off;		/* fragment offset field */
#define IP_RF 0x8000		/* reserved fragment flag */
#define IP_DF 0x4000		/* dont fragment flag */
#define IP_MF 0x2000		/* more fragments flag */
#define IP_OFFMASK 0x1fff	/* mask for fragmenting bits */
  u_char ip_ttl;		/* time to live */
  u_char ip_p;		/* protocol */
  u_short ip_sum;		/* checksum */
  struct in_addr ip_src;
  struct in_addr ip_dst; /* source and dest address */
};
#define IP_HL(ip)		(((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)		(((ip)->ip_vhl) >> 4)

	/* TCP header */
#define N2Hl(x) ntohl(x)
#define N2Hs(x) ntohs(x)
typedef u_int tcp_seq;

struct sniff_tcp {
  u_short th_sport;	/* source port */
  u_short th_dport;	/* destination port */
  tcp_seq th_seq;		/* sequence number */
  tcp_seq th_ack;		/* acknowledgement number */
  u_char th_offx2;	/* data offset, rsvd */
#define TH_OFF(th)	(((th)->th_offx2 & 0xf0) >> 4)
  u_char th_flags;
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
#define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
  u_short th_win;		/* window */
  u_short th_sum;		/* checksum */
  u_short th_urp;		/* urgent pointer */
};
struct sniff_udp {
  u_short src_port;
  u_short dst_port;
  u_short len;
  u_short cksm;
};
    
struct sniff_dns_header_t {
  uint16_t id;
  uint16_t flags;
    /* number of entries in the question section */
  uint16_t qdcount;
    /* number of resource records in the answer section */
  uint16_t ancount;
    /* number of name server resource records in the authority records section*/
  uint16_t nscount;
    /* number of resource records in the additional records section */
  uint16_t arcount;
};
struct sniff_dns_header_tcp_t {
  u_short len;
  struct sniff_dns_header_t normal_dns_header;
};
#define SIZE_ETHERNET 14
#define SIZE_UDP 8
const struct sniff_ethernet *ethernet; /* The ethernet header */
const struct sniff_ip *ip; /* The IP header */
const struct sniff_tcp *tcp; /* The TCP header */
const char *payload; /* Packet payload */

u_int size_ip;
u_int size_tcp;

#define DNS_QUERIE 0
#define DNS_RESPONSE 1