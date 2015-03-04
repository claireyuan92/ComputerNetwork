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


rip Table::makeReq(int interface_id){//undone
    
    rip rip;
    rip.command=1;
    rip.num_entries=myTable.size();
    for (map<uint32_t, Route>::iterator it=myTable.begin(); it!=myTable.end(); ++it) {
        if (it->second.nexthop==interface_id) {
            it->second.cost=MAX_COST+1;
        }
    }
    
    return rip;
}

rip Table::makeResp(int interface_id){//undone
    rip rip;
    rip.command=2;
    rip.num_entries=myTable.size();
    int i=0;
    for (map<uint32_t, Route>::iterator it=myTable.begin(); it!=myTable.end(); ++it) {
        rip.entries[i].address=it->first;
        rip.entries[i].cost=it->second.cost;
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
    cerr<<"Route Not Found"<<endl;
    return NULL;
}



void Table::OneSecT(){
    //for TTL--
    for (map<uint32_t, Route>::iterator it=myTable.begin(); it!=myTable.end(); ++it) {
        (it->second).TTL--;
        //check TTL<=0.if <= 0, cost = MAX
        if ( (it->second).TTL-- <= 0){
            (it->second).cost=MAX_COST+1;
        }
    }
}


//
void Table::update(rip rip,int interface_id){// Input Rip and where it comes from
    //for entries
    map<uint32_t,Route>::iterator it;
    
    for(uint16_t i=0; i<rip.num_entries; i++){
        //find ip
        uint32_t address = rip.entries[i].address;
        if((it=myTable.find(address))!= myTable.end()){
            //split horizon
            
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
