//
//  Node.cpp
//  Routing
//
//  Created by Claire on 2/27/15.
//  Copyright (c) 2015 Claire. All rights reserved.
//

#include "Node.h"



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
    int i=1;
    while (getline(&line,&linecap, f)>0){
        Interface myIF(i,line);
        interfaces.push_back(myIF);
        i++;
    }
    
    //3. Creating Routing table
        my_tbl.init(interfaces);
    
    
    //4. Building Server

    
    //5. new thread to receive --------not completed
    
    //6. Request route info
    request();
    
    //7. Response
    

}

void Node::c_ifconfig(){
    for (int i=0; i <interfaces.size(); i++) {
        
        cout<<(i+1)<<" "<<interfaces.at(i).configure()<<endl;
    }
}

void Node::c_routes(){
    
    my_tbl.printRoutes();
    
    
}

void Node::c_down(int interface_id){
    
    if (interface_id<1 || interface_id > interfaces.size()) {
        cout<<"Interface"<<interface_id<<"not found"<<endl;
        return;
    }
    
    interfaces[interface_id].setstatus(DOWN);
    
    cout<<"Interface "<<interface_id<<" down."<<endl;
}

void Node::c_up(int interface_id){
    
    if (interface_id<1 || interface_id > interfaces.size()) {
        cout<<"Interface"<<interface_id<<"not found"<<endl;
        return;
    }
    interfaces[interface_id].setstatus(UP);
    cout<<"Interface "<<interface_id<<" up."<<endl;
    
}

void Node::c_send(uint32_t dst_addr,string msg){
    
    //1. Choose Route
    Route myroute;
    myroute=my_tbl.selectRoute(dst_addr);
    
    //2. Pack
    uint32_t src=interfaces[myroute.interface_id].my_VIP;
    Packet mypack= pack( (void *)&msg, src,dst_addr,0);
    uint32_t nextHop=interfaces[myroute.interface_id].remote_VIP;
    uint32_t port= interfaces[myroute.interface_id].remote_port;
    
    //3.Send
    send(nextHop, port, &mypack);
    
    cout<<"send to "<<dst_addr<<" "<<msg<<"."<<endl;
    
}


void Node::request(){
    
    for (int i=0; i<interfaces.size(); i++) {
        RIP req_rip=my_tbl.makeReq();
        Packet rip=pack(&req_rip,interfaces[i].my_VIP, interfaces[i].remote_VIP,1);//RIP
        send(interfaces[i].remote_VIP,interfaces[i].remote_port, &rip);
    }
}

void Node::response(){
    for (int i=0; i<interfaces.size(); i++) {
        RIP req_rip=my_tbl.makeResp();
        Packet rip=pack(&req_rip,interfaces[i].my_VIP, interfaces[i].remote_VIP,1);//RIP
        send(interfaces[i].remote_VIP,interfaces[i].remote_port, &rip);
    }
}

Packet Node ::pack(void * payload,in_addr_t src,in_addr_t dst, int packet_type){
    
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
    src_ip.s_addr=src;
    packet.iph.ip_src=src_ip;
    in_addr dst_ip;
    dst_ip.s_addr=dst;
    packet.iph.ip_dst=dst_ip;
    packet.iph.ip_sum=0;
    packet.iph.ip_len=20+sizeof(payload);
    //packet.payload
    memcpy(&packet.payload, payload,sizeof(payload));
    //packet.iph.ip_sum=ip_sum((char*)&packet,(int)sizeof(packet.iph));
    return packet;
}

void Node:: send(uint32_t addr,int port,const void * packet) {
    
    struct sockaddr_in si_other;
    int s, slen=sizeof(si_other);
    
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
        cerr<<"socket"<<endl;;
        
        memset((char *) &si_other, 0, sizeof(si_other));
        si_other.sin_family = AF_INET;
        si_other.sin_port = htons(port);
        in_addr p;
    p.s_addr=addr;
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




//helpers

bool Node:: parseCmd(string cmd){
    
    //ifconfig
    if(!strcmp(cmd.c_str(),"ifconfig")){
        c_ifconfig();
        return true;
    }
    //route
    if(!strcmp(cmd.c_str(),"routes")){
        c_routes();
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
        c_down(id);
        return true;
    }
    if(!strncmp(cmd.c_str(),"up ",3)){
        int id;
        char buf[100];
        strcpy(buf,cmd.c_str());
        char * cmdid = strtok(buf," ");
        cmdid = strtok(NULL," ");
        id = atoi(cmdid);
        c_up(id);
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
        c_send(desn,cmd);
        return true;
    }
    
    cerr<<"wrong command"<<endl;
    return true;
}









