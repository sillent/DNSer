#include "main.h"

char *errbuf[256];

int open_capture() {
    char *dev;
    pcap_t *handle;
    
}

pcap_if_t *findDevice() {
    pcap_if_t *device;
    int retVal=pcap_findalldevs(&device,errbuf);
    if (retVal)
    {
        fprintf(stderr,"Cannot find any capturing device. Maybe not root ?");
        exit(EXIT_FAILURE);
    } else (retVal==-1) {
        perror(stderr,"Cannot find any capturing device. %s",errbuf);
        exit(EXIT_FAILURE);
    }
}