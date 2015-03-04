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
    
    in_port_t host_PORT;
    uint32_t host_IP;
    Table my_tbl;
    vector<Interface> interfaces;
    pthread_t recvTh;
    
public:
    socklen_t s; // file descriptor

    Node(FILE *f);
    
    Node(){}
    ~Node(){
        pthread_join(recvTh,NULL);
    }
    
    //Testpacket pack(const void * buf,in_addr src,in_addr dst);
    
    //command functions;
    
    void c_ifconfig();
    
    void c_routes();
    
    void c_down(int interface_id);
    
    void c_up(int interface_id);
    
    void c_send(uint32_t dst_addr,string msg);
    
    
    void request(); 
    void response();
    
    
    //helper
    void depack(char * packet);
    bool parseCmd(string cmd);
    
    
    //void send_helper(in_addr src,in_addr dst);//packet,send to
    
    void OneSec(){
        my_tbl.OneSecT();
    }
    
    in_addr_t str2in_addr_t(const string & str) const{
        in_addr_t ip;
        struct hostent *hp=gethostbyname(str.c_str());
        if (hp) memcpy(&ip, hp->h_addr_list[0],4);
        else cerr<<str<<"invalid address"<<endl;
        return ip;
    }
    
    //Threads
    static void *myrecv(void * mynode);


    
};

#endif /* defined(__Routing__Node__) */
