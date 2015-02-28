//
//  Interface.cpp
//  Routing
//
//  Created by Claire on 2/27/15.
//  Copyright (c) 2015 Claire. All rights reserved.
//

#include "Interface.h"

using namespace std;

bool Interface::upInterface(){
    int sock;
    char buf[BUFLEN];
    
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))< 0) {
            perror("Create socket error:");
                   return false;
                   
    }
    cout<<"Socket Created\n"<<endl;
    memset((char *) &sin, 0, sizeof(sin));
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_family =AF_INET;
    sin.sin_port=htons(remote_port);
    
 
    
}