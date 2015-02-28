//
//  main.cpp
//  Routing
//
//  Created by Claire on 2/27/15.
//  Copyright (c) 2015 Claire. All rights reserved.
//

#include <stdio.h>
#include <string.h>

#include <iostream>
#include <cstring>
#include <sstream>
#include <assert.h>
#include <cstdlib>
#include <list>
#include <vector>
#include <utility> //pair
#include <fstream>


#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Node.h"

<<<<<<< HEAD
using namespace std;
=======

>>>>>>> 213b3d8aa530fe8bad6663e9687a9c74220f411d

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
<<<<<<< HEAD
  cin.clear();
  string cmd;
  while(getline(cin,cmd)){
    cout<<cmd;
    mynode.parseCmd(cin);
    cin.clear();
  }
=======


>>>>>>> 213b3d8aa530fe8bad6663e9687a9c74220f411d
}
