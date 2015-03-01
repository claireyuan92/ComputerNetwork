//
//  Node.cpp
//  Routing
//
//  Created by Claire on 2/27/15.
//  Copyright (c) 2015 Claire. All rights reserved.
//

#include "Node.h"
#include <arpa/inet.h>

Node::Node(FILE *f){
    
    char *line = NULL;
    size_t linecap = 0;
    
    //1. assigning port
    if (getline(&line, &linecap, f)){
        struct hostent *hp;
        hp=gethostbyname(strtok(line, ":"));
        char *p=strdup(hp->h_addr);
        struct in_addr ip;
        inet_aton(p, &ip);
        host_IP=ip.s_addr;
        si_me.sin_port = htons(atoi(strtok(NULL, "\n")));
    }
    
    
    //2. Creating interfaces
    while (getline(&line,&linecap, f)>0){
        Interface myIF(line);
        interfaces.push_back(myIF);
    }
    
    //3. Creating Routing table
    my_tbl=new Table(interfaces);
    
    
    //4. Building Server

    socklen_t s;
    
    if((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
         perror("Create socket error:");
    
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if ((::bind(s, (struct sockaddr *)&si_me, sizeof(si_me))) < 0){
        perror("simplex-talk: bind");
        exit(1);
    }
    
    //5. new thread to receive --------not completed
    
    request();

}

void Node::c_ifconfig(){
    for (int i=0; i < interfaces.size(); i++) {
        
        cout<<i<<" "<<interfaces.at(i).configure()<<endl;
    }
}

void Node::c_routes(){
    
}

void Node::c_down(int interface_id){
    interfaces[interface_id].setstatus(0);
    
    cout<<"Interface "<<interface_id<<" down."<<endl;
}

void Node::c_up(int interface_id){
    
    interfaces[interface_id].setstatus(1);
    cout<<"Interface "<<interface_id<<" up."<<endl;
    
    
}



void Node::request(){
    
    for (int i=0; i<interfaces.size(); i++) {
     //   RIP req_rip=my_tbl->makeReq();
       // RIPpacket =pack(req_rip, <#in_addr src#>, <#in_addr dst#>)
     //   sockaddr_in si_other;
    }
}


//helpers

bool Node:: parseCmd(string cmd){
    
    //ifconfig
    if(!strcmp(cmd.c_str(),"ifconfig")){
        ifconfig();
        return true;
    }
    //route
    if(!strcmp(cmd.c_str(),"routes")){
        routes();
        return true;
    }
    //down
    if(!strncmp(cmd.c_str(),"down ",5)){
        int id;
        char buf[100];
        strcpy(buf,cmd.c_str());
        char * cmdid = strtok(buf," ");
        cmdid = strtok(NULL," ");
        //cout<<cmdid<<endl;
        //istringstream(cmdid)>>id;
        id = atoi(cmdid);
        down(id);
        return true;
    }
    if(!strncmp(cmd.c_str(),"up ",3)){
        int id;
        char buf[100];
        strcpy(buf,cmd.c_str());
        char * cmdid = strtok(buf," ");
        cmdid = strtok(NULL," ");
        id = atoi(cmdid);
        up(id);
        return true;
    }
    
    if(!strncmp(cmd.c_str(),"send ",5)){
        string des;
        string msg;
        cmd.erase(0,5);
        char buf[100];
        strcpy(buf,cmd.c_str());
        char * cmdip = strtok(buf," ");
        des = cmdip;
        cmd.erase(0,des.size()+1); 
        msg = cmd;
        struct in_addr ip;
        inet_aton(des.c_str(), &ip);
        uint32_t desn;
        send(desn,cmd);
        return true;
    }
    
    cout<<"wrong command"<<endl;
    return true;
}


RIPpacket Node::pack(RIP payload,in_addr src,in_addr dst){
    RIPpacket packet;
    packet.iph.ip_p=200;/* */
    packet.iph.ip_tos=0;
    packet.iph.ip_ttl=MAXTTL;
    packet.iph.ip_src=src;
    packet.iph.ip_dst=dst;
    packet.iph.ip_sum=0;
    packet.iph.ip_len=20+sizeof(payload);
    packet.payload=payload;
    return packet;
}

Testpacket Node::pack(string payload,in_addr src,in_addr dst){
    
    Testpacket packet;
    packet.iph.ip_p=0;/* protocol */
    packet.iph.ip_tos=0;
    packet.iph.ip_ttl=MAXTTL;
    packet.iph.ip_src=src;
    packet.iph.ip_dst=dst;
    packet.iph.ip_sum=0;
    packet.iph.ip_len=20+sizeof(payload);
    packet.payload=payload;
    packet.iph.ip_sum=ip_sum((char*)&packet,sizeof(packet.iph));
    return packet;
    
}


void * Node ::recv(void *socket_desc){
    
    char buf[BUFLEN];
    int s=*(int*)socket_desc;
    sockaddr si_other;
    socklen_t len=sizeof(si_other);
    if ((::recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&si_other,&len))<0){
        perror("recvfrom():");
        exit(1);
    }
    
    
    return NULL;
}

