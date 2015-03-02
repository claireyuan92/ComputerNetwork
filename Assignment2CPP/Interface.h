//
//  Interface.h
//  Routing
//
//  Created by Claire on 2/27/15.
//  Copyright (c) 2015 Claire. All rights reserved.
//

#ifndef __Routing__Interface__
#define __Routing__Interface__

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <vector>
#include <netdb.h>
#include <iostream>
#include <pthread.h>
#include <map>

#include "ipsum.h"


#define MTU 1400
#define BUFLEN 64*1024
#define DOWN 0//
#define UP 1//
#define MAX_TTL 12
#define MAX_COST 16

using namespace std;
//Route in Table
typedef struct{
    
    uint32_t cost;
    int interface_id;
    int TTL;
} Route;


struct Packet{
    ip iph;
    void * payload;
};


class Interface{
    
private:
    //UDP socket
   // sockaddr_in sin;
    //IP addresses/ports
    
    uint32_t remote_IP;
    int status;
    
public:
    
    int interface_id;
    int remote_port;
    uint32_t my_VIP;
    uint32_t remote_VIP;
    
    Interface(int interface_id, char *line);
    
    void send(int s, const void * packet);
    void setstatus(int stat);
    std::string configure();
    
    //Used for initializing the Table
    pair<uint32_t, Route> parseRoute(){
        Route rt;
        rt.cost=1;
        rt.interface_id=interface_id;
        rt.TTL=MAX_TTL;
         return pair<uint32_t, Route>(remote_VIP,rt);
    }

    Packet pack(void * payload,in_addr_t dst, int packet_type);
    
    
    
};


#endif /* defined(__Routing__Interface__) */
