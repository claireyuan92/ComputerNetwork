//
//  Node.cpp
//  Routing
//
//  Created by Claire on 2/27/15.
//  Copyright (c) 2015 Claire. All rights reserved.
//

#include "Node.h"
#include "Interface.h"


using namespace std;


Node::Node(FILE *f){
    
    char *line = NULL;
    size_t linecap = 0;
    
    //1. assigning port
    if (getline(&line, &linecap, f)){
        struct hostent *hp;
        hp=gethostbyname(strtok(line, ":"));
        host_IP=strdup(hp->h_addr);
        my_port=atoi(strtok(NULL, "\n"));
    }
    
    //2. creating interfaces
    while (getline(&line,&linecap, f)>0){
        Interface myIF(line);
        interfaces.push_back(myIF);
    }
}

//for debug
Node::Node(){}

void Node::ifconfig(){
    
}

void Node::routes(){
    
}
bool Node:: down(int interface_id){

  cout<<"Interface "<<interface_id<<" down."<<endl;
    return true;
}
bool Node:: up(int interface_id){
    int sock;
    
    cout<<"Interface "<<interface_id<<" up."<<endl;
    
    return true;
}

// defined as mysend to avoid confliction with existing function send
bool Node:: send(string addr,string msg){
  cout<<"send to "<<addr<<" "<<msg<<".";

    return true;
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

