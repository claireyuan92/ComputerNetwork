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
       // si_me.sin_addr=ip;
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
    Route *myroute;
    myroute=my_tbl.selectRoute(dst_addr);
    
    //2. Pack
    Packet mypack= interfaces[myroute->interface_id].pack( (void *)&msg,dst_addr,0);

    //3.Send
    interfaces[myroute->interface_id].send(s,&mypack);
    
    cout<<"send to "<<dst_addr<<" "<<msg<<"."<<endl;
    
}


void Node::request(){
    
    for (int i=0; i<interfaces.size(); i++) {
        RIP req_rip=my_tbl.makeReq();
        Packet rip=interfaces[i].pack(&req_rip,interfaces[i].remote_VIP,1);//RIP
        interfaces[i].send(s,&rip);
        //send(interfaces[i].remote_VIP,interfaces[i].remote_port, &rip);
    }
}

void Node::response(){
    for (int i=0; i<interfaces.size(); i++) {
        RIP req_rip=my_tbl.makeResp();
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
        //        checkRIP();
                return;
            }
            else if(mypack->iph.ip_p==0){
                cout<<(char *)(mypack->payload)<<endl;
                return;
            }
            else{
                cerr<<"Not a Valid pack"<<endl;
            }
        }
    }
    
    //Forwarding
    
    Route * myrt;
    if((myrt=my_tbl.selectRoute(mypack->iph.ip_dst.s_addr))!=NULL){
        --(mypack->iph.ip_ttl);
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
        struct in_addr ip;
        inet_aton(des.c_str(), &ip);
        uint32_t desn;
        c_send(desn,cmd);
        return true;
    }
    
    cerr<<"wrong command"<<endl;
    return true;
}









