/* 
 * File:   listener.h
 * Author: santa
 *
 * Created on 24 июля 2015 г., 15:56
 */

#ifndef LISTENER_H
#define	LISTENER_H

#include "worker.h"
struct mesg_t {
    char mes[3];
    long long arg;
};
#define SERVER_PORT 33990
void * listener();
int sendmsgto(struct sockaddr_in to, uint64_t mesage);
void grepp(struct mesg_t data,struct sockaddr_in respondto);
void doStat();
#endif	/* LISTENER_H */

