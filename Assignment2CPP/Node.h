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

#define h_addr h_addr_list[0] /* for backward compatibility */

using namespace std;

class Node{
    private:
        
    char* host_IP;
    int my_port;
    
    vector<Interface> interfaces;
    public:
    Node(FILE *f);
    void ifconfig();
    
    void routes();
    
    bool down(int interface_id);
    
    bool up(int interface_id);
    
    // defined as mysend to avoid confliction with existing function send
    bool send(const char * addr,char * msg);
    bool parseCmd(string cmd);
    
};

#endif /* defined(__Routing__Node__) */
