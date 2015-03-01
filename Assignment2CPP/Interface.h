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

#define MTU 1400
#define BUFLEN 64*1024
#define DOWN 0//
#define UP 1//

//Route in Table
typedef struct{
    
    uint32_t cost;
    int interface_id;
    int TTL;
} Route;

using namespace std;


class Interface{
    
private:
    //UDP socket
    sockaddr_in sin;
    //IP addresses/ports
    int remote_port;
    uint32_t remote_IP;
    int status;
    
public:
    
    int interface_id;
    uint32_t my_VIP;
    uint32_t remote_VIP;
    Interface(int interface_id, char *line);
    
    void setstatus(int stat);
    
    std::string configure();
    
     pair<uint32_t, Route> parseRoute(){
        Route rt;
        rt.cost=1;
        rt.interface_id=interface_id;
         return pair<uint32_t, Route>(remote_VIP,rt);
    }
    
    
    
};


#endif /* defined(__Routing__Interface__) */
