#include "main.h"
#include <string.h>
int find_device(char *devName) {
//    struct pcap_if *iface;
    pcap_if_t *devices=find_devices();   // search all available device
    while( devices!=NULL) {
      if (strcmp(devices->name,devName)==0) {
        return DEVFND;
      }
      devices=devices->next;
    }
    pcap_freealldevs(devices);  // free all finded devs
    return DEVNF;
}

pcap_if_t *find_devices() {
    pcap_if_t *device;
    int retVal=pcap_findalldevs(&device,errbuf);
    if (retVal)
    {
        fprintf(stderr,"Cannot find any capturing device. %s\n", errbuf);
        return NULL;
    } else {
        return device;
    }
}
