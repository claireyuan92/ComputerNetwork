//
//  Interface.h
//  Routing
//
//  Created by Claire on 2/27/15.
//  Copyright (c) 2015 Claire. All rights reserved.
//

#ifndef __Routing__Interface__
#define __Routing__Interface__

#include <stdio.h>
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
#include <iostream>

#define MTU (1400*8)
#define BUFLEN (64*1024*8)

class Interface{
    
private:
    //UDP socket
    sockaddr_in sin;
    //IP addresses/ports
    int remote_port;
    string my_VIP;
    string remote_VIP;
    string remote_IP;
    
public:
    
    Interface(char *line){
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
    
    bool upInterface();
};


#endif /* defined(__Routing__Interface__) */
