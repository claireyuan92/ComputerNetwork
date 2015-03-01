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
#include "ipsum.h"
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
    Table my_tbl;
    vector<Interface> interfaces;
    
public:
    
    Node(FILE *f);
    
    Node();
    ~Node(){
    }
    
    RIPpacket pack(RIP payload,in_addr_t src,in_addr_t dst);
    
  //  Testpacket pack(string payload,in_addr src,in_addr dst);
    
    //command functions;
    
    void c_ifconfig();
    
    void c_routes();
    
    void c_down(int interface_id);
    
    void c_up(int interface_id);
    
    void c_send(uint32_t addr,string msg){
        cout<<"send to "<<addr<<" "<<msg<<"."<<endl;
        
    }
    template<typename T>
    void send(uint32_t addr,T packet);
    
    
    //new thread
    void *recv(void *socket_desc);

    void request(); 
    void response();
    
    //helpers
    bool parseCmd(string cmd);
    void send_helper(in_addr src,in_addr dst);//packet,send to
    

    
};

#endif /* defined(__Routing__Node__) */
