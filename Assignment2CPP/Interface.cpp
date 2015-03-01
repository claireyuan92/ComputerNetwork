//
//  Interface.cpp
//  Routing
//
//  Created by Claire on 2/27/15.
//  Copyright (c) 2015 Claire. All rights reserved.
//

#include "Interface.h"



using namespace std;

Interface::Interface(int myid, char *line){
    interface_id=myid;
    char *pch;
    struct hostent *hp;
    hp=gethostbyname(strtok(line, ":"));
    in_addr ip;
    inet_aton(strdup(hp->h_addr), &ip);
    remote_IP=ip.s_addr;
    
    pch = strtok (NULL, " ");
    
    remote_port=atoi(pch);
    
    pch= strtok (NULL, " ");
    inet_aton(pch, &ip);
    my_VIP = ip.s_addr;
    
    pch= strtok (NULL, " ");
    inet_aton(pch, &ip);
    remote_VIP = ip.s_addr;
    
    /*
    int sock;
    char buf[BUFLEN];
    
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))< 0) {
        perror("Create socket error:");

        
    }
    
    cout<<"Socket Created\n"<<endl;
    memset((char *) &sin,mmnmnmnmnbnb m 0, sizeof(sin));
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_family =AF_INET;
    sin.sin_port=htons(remote_port)
     */
}

void Interface:: setstatus(int stat){
    status=stat;
}

string Interface::configure(){
  
    in_addr s;
    s.s_addr=my_VIP;
    string result = string(inet_ntoa(s)) + (status? (" up" ): (" down"));
    return result;
}



