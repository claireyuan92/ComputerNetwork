//
//  Node.cpp
//  Routing
//
//  Created by Claire on 2/27/15.
//  Copyright (c) 2015 Claire. All rights reserved.
//

#include "Node.h"

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
        interface myIF(line);
        interfaces.push_back(myIF);
    }
}

void Node::ifconfig(){
    
}

void Node::routes(){
    
}
bool Node:: down(int interface_id){
    return true;
}
bool Node:: up(int interface_id){
    return true;
}

// defined as mysend to avoid confliction with existing function send
bool Node:: send(const char * addr,char * msg){
    return true;
}

bool Node:: parseCmd(string cmd){

  //ifconfig
  if(!strcmp(cmd,"ifconfig")){
    ifconfig();
    return TRUE;
  }
  //route
  if(!strcmp(cmd,"route")){
    route();
    return TRUE;
  }
      //down
  if(!strncmp(cmd,"down ",5)){
    int id;
    istringstream(strtok(cmd.c_str()," "))>>id;
    mynode.down(id);
    return TURE;
  }
  if(!strcmp(cmd,"up ",3)){
 
  }
	
  if(!strcmp(cmd,"send ",5)){

  }
    
  cout<<"wrong command"<<endl;
  return TRUE;
}

