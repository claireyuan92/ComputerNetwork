//
//  Node.h
//  Routing
//
//  Created by Claire on 2/27/15.
//  Copyright (c) 2015 Claire. All rights reserved.
//

#ifndef __Routing__Node__
#define __Routing__Node__

#include "Interface.h"
#include "Table.h"
#include <netinet/ip.h>

#define h_addr h_addr_list[0] /* for backward compatibility */

/*
typedef struct {
    NodeAddr Destination;
    NodeAddr NextHop;
    int Cost;
    u_short TTL;
} Route;
*/

using namespace std;

struct RIPpacket{
    ip iph;
    RIP payload;
};


struct Testpacket{
    ip iph;
    string payload;
};

class Node{
    
private:
    
    uint32_t host_IP;
    sockaddr_in si_me;// server
    Table * my_tbl;
    vector<Interface> interfaces;
    
public:
    
    Node(FILE *f);
    
    Node();
    
    RIPpacket pack(RIP payload,in_addr src,in_addr dst);
    
    Testpacket pack(string payload,in_addr src,in_addr dst);
    
    void ifconfig();
    
    void routes();
    
    void down(int interface_id);
    
    void up(int interface_id);
    
    void send(string addr,string msg);
    
    //new thread
    void *recv(void *socket_desc);

    void request(); 
    void response();
    
    //helpers
    bool parseCmd(string cmd);
    void send_helper(in_addr src,in_addr dst);//packet,send to
    

    
};

#endif /* defined(__Routing__Node__) */
