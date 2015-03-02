//
//  Node.h
//  Routing
//
//  Created by Claire on 2/27/15.
//  Copyright (c) 2015 Claire. All rights reserved.
//

#ifndef __Routing__Node__
#define __Routing__Node__


#include "Table.h"


//#define h_addr h_addr_list[0] /* for backward compatibility */


class Node{
    
private:
    
    uint32_t host_IP;
    int host_PORT;
    Table my_tbl;
    vector<Interface> interfaces;
    
public:
    socklen_t s; // server socket

    Node(FILE *f);
    
    Node(){}
    
    //Testpacket pack(const void * buf,in_addr src,in_addr dst);
    
    //command functions;
    
    void c_ifconfig();
    
    void c_routes();
    
    void c_down(int interface_id);
    
    void c_up(int interface_id);
    
    void c_send(uint32_t dst_addr,string msg);
    
    //void send(uint32_t addr,int port,const void * buf);
    
    
    void request(); 
    void response();
    
    
    //helper
    void depack(char * packet);
    bool parseCmd(string cmd);
    //void send_helper(in_addr src,in_addr dst);//packet,send to
    
    void OneSec(){
        my_tbl.OneSecT();
    }
    
};

#endif /* defined(__Routing__Node__) */
