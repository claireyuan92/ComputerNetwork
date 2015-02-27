//
//  Node.h
//  Routing
//
//  Created by Claire on 2/27/15.
//  Copyright (c) 2015 Claire. All rights reserved.
//

#ifndef __Routing__Node__
#define __Routing__Node__

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <stdbool.h>

using namespace std;

class Node{
    private:
    class interface{
        private:
        //UDP socket
        sockaddr_in sin;
        //IP addresses/ports
        int remote_port;
        char * my_VIP;
        char * remote_VIP;
        public:
        interface(char *line){
            char *pch;
            pch = strtok (line, ":");
            //host_IP=pch;
            pch = strtok (NULL, " ");
            
            remote_port=atoi(pch);
            
            pch= strtok (NULL, " ");
            my_VIP = pch;
            
            pch= strtok (NULL, " ");
            remote_VIP = pch;
        }
    };
    
    char * host_IP;
    int my_port;
    vector<interface> interfaces;
    
    public:
    Node(FILE *f);
    void ifconfig();
    
    void routes();
    
    bool down(int interface_id);
    
    bool up(int interface_id);
    
    // defined as mysend to avoid confliction with existing function send
    bool send(const char * addr,char * msg);

    
};

#endif /* defined(__Routing__Node__) */
