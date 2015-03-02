//
//  main.cpp
//  Routing
//
//  Created by Claire on 2/27/15.
//  Copyright (c) 2015 Claire. All rights reserved.
//



#include "Node.h"


#define MAX_ROUTES 200
#define MAX_TTL 120

void *cmdReader(void *);

void * myrecv(void * mynode);


int main(int argc, const char * argv[]) {
    
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
    
    //Listen from others
    /*
    pthread_t recvTh;
    pthread_create (&recvTh, NULL, myrecv, (void*) &mynode);
    pthread_join(recvTh,NULL);
    pthread_detach(recvTh);
    */
    
    //Open Command thread
    
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

void * myrecv(void * mynode){
    
    cout<<"enter recv";
    Node * node = (Node *) mynode;
    socklen_t s;
    sockaddr_in si_me, si_other;
    if((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
        perror("Create socket error:");
    
    memset((char *) &si_me, 0, sizeof(si_me));
    
    si_me.sin_family = AF_INET;
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if ((::bind(s, (struct sockaddr *)&si_me, sizeof(si_me))) < 0){
        perror("simplex-talk: bind");
        exit(1);
    }
    
    char buf[BUFLEN];
    
    socklen_t len=sizeof(si_other);
    
    while((::recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&si_other,&len))){
            node->depack(buf);
    }
    
    return NULL;
}


