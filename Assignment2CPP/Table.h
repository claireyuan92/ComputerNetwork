
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
    vector<Entry> entries;
    
} RIP;

class Table{
    
private:
    
    map<uint32_t, Route> myTable;
    
public:
    Table(){}
    void init(vector<Interface> interfaces){
        
        for (vector<Interface>:: iterator it=interfaces.begin(); it!=interfaces.end(); ++it) {
 
            myTable.insert(it->parseRoute());
        }
    }
    
    void update(RIP rip);
    
    RIP makeReq(){
        
        RIP rip;
        rip.command=1;
        rip.num_entries=myTable.size();
        for (map<uint32_t, Route>::iterator it=myTable.begin(); it!=myTable.end(); ++it) {
            Entry etr;
            etr.address=it->first;
            etr.cost=it->second.cost;
            rip.entries.push_back(etr);
        }
        return rip;
    }
    RIP makeResp(int interface_id){
        RIP rip;
        return rip;
    }
    
};


#endif /* defined(__Table__) */
