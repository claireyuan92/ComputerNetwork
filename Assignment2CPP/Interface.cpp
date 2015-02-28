//
//  Interface.cpp
//  Routing
//
//  Created by Claire on 2/27/15.
//  Copyright (c) 2015 Claire. All rights reserved.
//

#include "Interface.h"

using namespace std;

Interface::Interface(char *line){
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
    
    /*
    int sock;
    char buf[BUFLEN];
    
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))< 0) {
        perror("Create socket error:");

        
    }
    
    cout<<"Socket Created\n"<<endl;
    memset((char *) &sin, 0, sizeof(sin));
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_family =AF_INET;
    sin.sin_port=htons(remote_port)
     */
}

void Interface:: setstatus(int stat){
    status=stat;
}

string Interface::configure(){
    
    string result = my_VIP + status? " up" : " down";
    return result;
}



