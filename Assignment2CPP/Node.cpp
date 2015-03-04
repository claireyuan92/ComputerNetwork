//
//  Node.cpp
//  Routing
//
//  Created by Claire on 2/27/15.
//  Copyright (c) 2015 Claire. All rights reserved.
//

#include "Node.h"


 void * Node::myrecv(void * mynode){
    
    cout<<"enter recv";
    Node * node = (Node *) mynode;

    sockaddr_in si_other;
    char buf[BUFLEN];
    socklen_t len=sizeof(si_other);
    
    while((::recvfrom(node->s, buf, BUFLEN, 0, (struct sockaddr *)&si_other,&len))){
        cout<<"RECEIVING"<<endl;
        node->depack(buf);
    }
    
    return NULL;
}

Node::Node(FILE *f){
    
    char *line = NULL;
    size_t linecap = 0;
    
    
    //1. assigning port and host
    if (getline(&line, &linecap, f)){
        host_IP=str2in_addr_t(strtok(line, ":"));
        host_PORT = htons(atoi(strtok(NULL, "\n")));
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
    
    
    //4. Building a Server
    
    sockaddr_in si_me;
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
    pthread_create (&recvTh, NULL, myrecv, this);
    //pthread_detach(recvTh);
    
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
    Route *myroute;
    myroute=my_tbl.selectRoute(dst_addr);
    
    //2. Pack
    Packet mypack= interfaces[myroute->nexthop-1].pack( (void *)&msg,dst_addr,0);

    //3.Send
    interfaces[myroute->nexthop-1].send(s,&mypack);
    
    cout<<"send to "<<dst_addr<<" "<<msg<<"."<<endl;
    
}


void Node::request(){
    
    for (int i=0; i<interfaces.size(); i++) {
        RIP req_rip=my_tbl.makeReq(i);
        Packet rip=interfaces[i].pack(&req_rip,interfaces[i].remote_VIP,1);//RIP
        interfaces[i].send(s,&rip);
        //send(interfaces[i].remote_VIP,interfaces[i].remote_port, &rip);
    }
}

void Node::response(){
    for (int i=0; i<interfaces.size(); i++) {
        RIP req_rip=my_tbl.makeResp(i);
        Packet rip=interfaces[i].pack(&req_rip,interfaces[i].remote_VIP,1);//RIP
        interfaces[i].send(s,&rip);
        //send(interfaces[i].remote_VIP,interfaces[i].remote_port, &rip);
    }
}



void Node::depack(char * pack){
    Packet * mypack;
    memcpy(mypack, &pack, sizeof(pack));
    //Local Delivery
    for (vector<Interface> ::iterator it=interfaces.begin(); it!=interfaces.end(); ++it) {
        if (it->my_VIP==mypack->iph.ip_dst.s_addr) {
            //RIP
            if (mypack->iph.ip_p==200) {
                //check command
                if (((RIP *)(mypack->payload))->command==1) {//Request
                    response();
                }
              //  my_tbl.update()
                return;
            }
            //test data
            else if(mypack->iph.ip_p==0){
                cout<<(char *)(mypack->payload)<<endl;
                return;
            }
            else{
                cerr<<"Not a Valid pack"<<endl;
            }
        }
    }
    
    //Forwarding INCOMPLETE
    
    Route * myrt;
    if((myrt=my_tbl.selectRoute(mypack->iph.ip_dst.s_addr))!=NULL){
        --(mypack->iph.ip_ttl);
        interfaces[myrt->nexthop].send(s, mypack);
    }
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
        c_send(str2in_addr_t(des),cmd);
        return true;
    }
    
    cerr<<"wrong command"<<endl;
    return true;
}









