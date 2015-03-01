//
//  main.cpp
//  Routing
//
//  Created by Claire on 2/27/15.
//  Copyright (c) 2015 Claire. All rights reserved.
//

#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Node.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>

#include<pthread.h> //for threading , link with lpthread



#define MAX_ROUTES 200
#define MAX_TTL 120

void *cmdReader(void *);




int main(int argc, const char * argv[]) {
  /*      
    if(argc!=2){
        printf("Usage: Take one file as commandline input\n");
        return 1;
    }
    
    FILE *f = fopen("/Users/Meng/ComputerNetwork/Assignment2/BInput","r");
    if(f==NULL){
        perror("Cannot open file ");
        return 1;
    }
    // Create NODE
    
    Node mynode(f);
  */
  Node mynode;

    pthread_t cmdTh;
    if( pthread_create( &cmdTh , NULL ,  cmdReader , (void*) &mynode) < 0){
            perror("error: create thread");
            return 1;
        }
	cout<<"after create thread\n";
	pthread_join(cmdTh,NULL);
      	pthread_detach(cmdTh);
	cout<<"detach\n";
}


void * cmdReader(void * mynode){
  //cout<<"cmdReader\n";
  string cmd;
  Node * node = (Node *) mynode;
  while(getline(cin,cmd)){
    //cout<<cmd<<endl;
     (* node).parseCmd(cmd);
    cin.clear();
  }
  pthread_exit(NULL);
}


