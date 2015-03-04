//
//  main.cpp
//  Routing
//
//  Created by Claire on 2/27/15.
//  Copyright (c) 2015 Claire. All rights reserved.
//



#include "Node.h"

#include <unistd.h>//for sleep

#define MAX_ROUTES 200
//#define MAX_TTL 120
#define RES_TIME 5
//#define EXP_TIME 12

void *cmdReader(void *);

void * myrecv(void * mynode);

void *timer(void *);


int main(int argc, const char * argv[]) {
    
     if(argc!=2){
     printf("Usage: Take one file as commandline input\n");
     return 1;
     }
    
    
     FILE *f = fopen("./AInput","r");
     if(f==NULL){
     perror("Cannot open file ");
     return 1;
     }
     // Create NODE
     
     Node mynode(f);

    
    //Listen from others
    /*
     pthread_t recvTh;
     pthread_create (&recvTh, NULL, myrecv, (void*) &mynode);
     //pthread_join(recvTh,NULL);//????????
     pthread_detach(recvTh);
    */
    
    //Open Command thread
    /*
    pthread_t timerTh;
    if( pthread_create( &timerTh , NULL ,  timer , (void*) &mynode) < 0){
        perror("error: create thread");
        return 1;
    }
    //pthread_join(cmdTh,NULL);
    
    pthread_detach(timerTh);
    */
    
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
        node->parseCmd(cmd);
        cin.clear();
    }
    pthread_exit(NULL);
}



void * timer(void * mynode){
    Node * node = (Node *) mynode;
    while(1){
        for (int i = 0; i < RES_TIME; i++){
            sleep(1);
            node->OneSec();
            //cout<<"wakeup"<<i<<endl;
        }
        node->response();
    }
   // pthread_exit(NULL);
}


