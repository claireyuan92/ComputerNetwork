
#ifndef __Table__
#define __Table__

#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <stdbool.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#include <map>
#include "Interface.h"



using namespace std;



// RIP packet
typedef struct{
    uint32_t cost;
    uint32_t address;
} Entry;

typedef struct{
    uint16_t command;
    uint16_t num_entries;
    Entry * entries;
    
} RIP;

class Table{
    
private:
    
    map<uint32_t, Route> myTable;
    
public:
    Table();
    void init(vector<Interface> interfaces){
        
        for (vector<Interface>:: iterator it; it!=interfaces.end(); ++it) {
 
            myTable.insert(it->parseRoute());
        }
    }
    
    void update(RIP rip);
    RIP makeReq();
    RIP makeResp(int interface_id);
    
};


#endif /* defined(__Table__) */
