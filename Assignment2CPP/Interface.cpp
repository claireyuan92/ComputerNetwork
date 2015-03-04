


//
//  Interface.cpp
//  Routing
//
//  Created by Claire on 2/27/15.
//  Copyright (c) 2015 Claire. All rights reserved.
//

#include "Interface.h"

#include "ipsum.h"

Interface::Interface(int myid, char *line){
    
    interface_id=myid;

    remote_IP=str2in_addr_t(strtok(line, ":"));
    
    
    remote_port=htons(atoi(strtok (NULL, " ")));
    
    my_VIP = inet_addr(strtok(NULL , " "));
    
    //str2in_addr_t(strtok(NULL , " "));

    
    remote_VIP =inet_addr(strtok(NULL , " "));
    
    status=UP;

    
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
    
    
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    //si_other.sin_port = remote_port;
    si_other.sin_port = htons(17001);

    si_other.sin_addr.s_addr = remote_IP;

    /*if (inet_aton("127.0.0.1" , &si_other.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    */
    
    //NEED FRAGMENTATION
    
    char buf[MTU];
    memcpy(buf, packet, sizeof(packet));
    
    if (sendto(s,buf, sizeof(buf), 0,(struct sockaddr*) &si_other, slen)==-1)
      cerr<<"sendto()"<<endl;
    
    //close(s);
    
    cout<<"send"<<si_other.sin_port<<endl;
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




