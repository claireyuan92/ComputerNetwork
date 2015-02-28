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
        host_IP=strdup(hp->h_addr);
        si_me.sin_port = htons(atoi(strtok(NULL, "\n")));
    }
    
    //2. creating interfaces
    while (getline(&line,&linecap, f)>0){
        Interface myIF(line);
        interfaces.push_back(myIF);
    }
    
    //3. Building Server
    sockaddr_in si_other;
    socklen_t s, slen=sizeof(si_other);
    
    char buf[BUFLEN];
    
    if((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
         perror("Create socket error:");
    
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if ((::bind(s, (struct sockaddr *)&si_me, sizeof(si_me))) < 0){
        perror("simplex-talk: bind");
        exit(1);
    }
    /*
    
    if ((::recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&si_other, &slen))<0){
         perror("recvfrom():");
         exit(1);
    }
    */
 

    

}

void Node::ifconfig(){
    for (int i=0; i < interfaces.size(); i++) {
        
        cout<<i<<" "<<interfaces.at(i).configure()<<endl;
    }
}

void Node::routes(){
    
}

void Node:: down(int interface_id){
    interfaces[interface_id].setstatus(0);
    
    cout<<"Interface "<<interface_id<<" down."<<endl;
}

void Node:: up(int interface_id){
    
    interfaces[interface_id].setstatus(1);
    cout<<"Interface "<<interface_id<<" up."<<endl;
    
    
}

void Node:: send(string  addr,string msg){
    
    cout<<"send to "<<addr<<" "<<msg<<"."<<endl;
    
}


//helpers
void Node::recv(){
    
}


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
        send(des,cmd);
        return true;
    }
    
    cout<<"wrong command"<<endl;
    return true;
}



void forward();