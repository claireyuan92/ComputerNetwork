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


RIP Table::makeReq(int interface_id){//undone
    
    RIP rip;
    rip.command=1;
    rip.num_entries=myTable.size();
    for (map<uint32_t, Route>::iterator it=myTable.begin(); it!=myTable.end(); ++it) {
        if (it->second.nexthop==interface_id) {
            it->second.cost=MAX_COST+1;
        }
    }
    
    return rip;
}

RIP Table::makeResp(int interface_id){//undone
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
        cout<<p<<" "<<it->second.nexthop<<" "<<it->second.cost<<endl;
    }
    
}

Route * Table::selectRoute(uint32_t addr){
    
    for (map<uint32_t, Route>::iterator it=myTable.begin(); it!=myTable.end(); ++it) {
        if (it->first==addr) {
            return &it->second;
        }
    }
    return NULL;
}



void Table::OneSecT(){
    //for TTL--
    for (map<uint32_t, Route>::iterator it=myTable.begin(); it!=myTable.end(); ++it) {
        (it->second).TTL--;
        //check TTL<=0.if <= 0, cost = MAX
        if ( (it->second).TTL-- <= 0){
            (it->second).cost=MAX_COST;
        }
    }
}


void Table::update(RIP rip,int interface_id){
    //for entries
    map<uint32_t,Route>::iterator it;
    for(uint16_t i; i<rip.num_entries; i++){
        //find ip
        uint32_t address = rip.entries[i].address;
        if((it=myTable.find(address))!= myTable.end()){
            //compare cost
            if (it->second.cost >= (rip.entries[i].cost+1)) {
                it->second.cost = rip.entries[i].cost+1;
                it->second.TTL = MAX_TTL;
                it->second.nexthop = interface_id;
            }
        }
        //add Route
        else{
            Route newR;
            newR.cost = rip.entries[i].cost+1;
            newR.nexthop = interface_id;
            newR.TTL = MAX_TTL;
            myTable.insert(pair<uint32_t,Route >(address,newR));
            
        }
    }
    
}
