//
//  Interface.cpp
//  Routing
//
//  Created by Claire on 2/27/15.
//  Copyright (c) 2015 Claire. All rights reserved.
//

#include "Interface.h"


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


void Interface:: send(int s,const void * packet) {
    
    struct sockaddr_in si_other;
    int slen=sizeof(si_other);
    
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
        cerr<<"socket"<<endl;;
    
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(remote_port);
    in_addr p;
    p.s_addr=remote_IP;
    si_other.sin_addr=p;
    
    //NEED FRAGMENTATION
    
    char buf[MTU];
    memcpy(buf, packet, sizeof(packet));
    
    if (sendto(s, buf, sizeof(buf), 0,(struct sockaddr*) &si_other, slen)==-1)
        cerr<<"sendto()"<<endl;
    
    close(s);
    
    cout<<"send"<<endl;
    return;
}

Packet Interface::pack(void * payload,in_addr_t dst, int packet_type){
    
    Packet packet;
    if (packet_type==1) {
        packet.iph.ip_p=200;/* protocol */
    }
    else if(packet_type==0){
        packet.iph.ip_p=0;/* protocol */
    }
    else{
        cerr<<"Packet type unknown"<<endl;
    }
    
    packet.iph.ip_tos=0;//not sure
    packet.iph.ip_ttl=MAXTTL;
    in_addr src_ip;
    src_ip.s_addr=my_VIP;
    packet.iph.ip_src=src_ip;
    in_addr dst_ip;
    dst_ip.s_addr=dst;
    packet.iph.ip_dst=dst_ip;
    packet.iph.ip_sum=0;
    packet.iph.ip_len=20+sizeof(payload);
    
    //packet.payload
    memcpy(&packet.payload, payload,sizeof(payload));
    packet.iph.ip_sum=ip_sum((char*)&packet,(int)sizeof(packet.iph));
    return packet;
}




