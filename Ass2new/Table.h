
#ifndef __Table__
#define __Table__



#include "Interface.h"




class Table{
    
private:
    //Table uses VIP
    map<uint32_t, Route> myTable;
    
public:

    void init(vector<Interface> interfaces);
    
    void update(RIP rip, int interface_id);
    
    
    RIP makeReq(int interface_id);
    
    
    RIP makeResp(int interface_id);
    
    void printRoutes();
    
    Route * selectRoute(uint32_t addr);
    
    void OneSecT();
};


#endif /* defined(__Table__) */
