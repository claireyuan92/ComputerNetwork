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
#include <netdb.h>
#include <arpa/inet.h>

#define h_addr h_addr_list[0] /* for backward compatibility */

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
        char * remote_IP;
        public:
        interface(char *line){
            char *pch;
            
            struct hostent *hp;
            hp=gethostbyname(strtok(line, ":"));
            remote_IP=strdup(hp->h_addr);
   
            pch = strtok (NULL, " ");
            
            remote_port=atoi(pch);
            
            pch= strtok (NULL, " ");
            my_VIP = pch;
            
            pch= strtok (NULL, " ");
            remote_VIP = pch;
        }
    };
    
    char* host_IP;
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
