
#ifndef __Table__
#define __Table__



#include "Interface.h"




class Table{
    
private:
    //Table uses VIP
    map<uint32_t, Route> myTable;
    
public:

    void init(vector<Interface> interfaces);
    
    void update(rip rip, int interface_id);
    
    
    rip makeReq(int interface_id);
    
    
    rip makeResp(int interface_id);
    
    void printRoutes();
    
    Route * selectRoute(uint32_t addr);
    
    void OneSecT();
    
    void removeRoute(int down_interface_id){
        for (map<uint32_t,Route>:: iterator it=myTable.begin(); it!=myTable.end(); ++it) {
            if (it->second.nexthop==down_interface_id) {
                myTable.erase(it);
                return;
            }
        }
        cerr<<"Interface doesn't exist"<<endl;
    }
};


#endif /* defined(__Table__) */
