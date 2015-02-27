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
        host_IP=strtok(line, ":");
        my_port=atoi(strtok(NULL, "\n"));
    }
    
    //2. creating interfaces
    
    while (getline(&line,&linecap, f)>0){
        interface myIF(line);
        interfaces.push_back(myIF);
    }
    
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