//
//  Table.cpp
//  Routing
//
//  Created by Claire on 2/28/15.
//  Copyright (c) 2015 Claire. All rights reserved.
//

#include "Table.h"

void Table::init(vector<Interface> interfaces){
    
    for (vector<Interface>:: iterator it=interfaces.begin(); it!=interfaces.end(); ++it) {
        
        myTable.insert(it->parseRoute());
    }
}


RIP Table::makeReq(){
    
    RIP rip;
    rip.command=1;
    rip.num_entries=myTable.size();
    return rip;
}

RIP Table::makeResp(){
    RIP rip;
    rip.command=2;
    rip.num_entries=myTable.size();
    for (map<uint32_t, Route>::iterator it=myTable.begin(); it!=myTable.end(); ++it) {
        Entry etr;
        etr.address=it->first;
        etr.cost=it->second.cost;
        rip.entries.push_back(etr);
    }
    
    return rip;
}

void Table::printRoutes(){
    for (map<uint32_t, Route>::iterator it=myTable.begin(); it!=myTable.end(); ++it) {
        in_addr add;
        add.s_addr=it->first;
        char * p= inet_ntoa(add);
        cout<<p<<" "<<it->second.interface_id<<" "<<it->second.cost<<endl;
    }
    
}

Route Table::selectRoute(uint32_t addr){
    Route res;
    return res;
}