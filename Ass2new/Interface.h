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
    int nexthop;
    int TTL;
} Route;


typedef struct {
    ip iph;
    char payload[MTU-sizeof(ip)] ;
    
}Packet;


// RIP packet
/*
typedef struct{
    uint32_t cost;
    uint32_t address;
    
} Entry;

typedef struct{
    uint16_t command;
    uint16_t num_entries;
    vector<Entry> entries;
    
} RIP;

 */
struct rip{
    uint16_t command;
    uint16_t num_entries;
    struct {
        uint32_t cost;
        uint32_t address;
    } entries[64];
};


class Interface{
    
public:
    
    int interface_id;
    int status;
    
    in_port_t remote_port; 
    uint32_t remote_IP;
    uint32_t my_VIP;
    uint32_t remote_VIP;
    
    Interface(int interface_id, char *line);
    
    void send(int s, Packet packet);//get fd from Node
    void setstatus(int stat);
    std::string configure();
    
    //Used for initializing the Table
    pair<uint32_t, Route> parseRoute(){
        Route rt;
        rt.cost=1;
        rt.nexthop=interface_id;
        rt.TTL=MAX_TTL;
         return pair<uint32_t, Route>(remote_VIP,rt);
    }

  Packet pack(const char * payload,in_addr_t dst, int packet_type);
    
    in_addr_t str2in_addr_t(const string & str) const{
        in_addr_t ip;
        struct hostent *hp=gethostbyname(str.c_str());
        if (hp) memcpy(&ip, hp->h_addr_list[0],4);
        else cerr<<str<<"invalid address"<<endl;
        return ip;
    }

    
};


#endif /* defined(__Routing__Interface__) */
