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

#define MAX_ROUTES 200
#define MAX_TTL 120



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


}
