
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

#sys/socket.h
netinet/inet.h
arpa/inet.h


#define 


typedef struct{
  uint32_t cost;
  unit32_t address;
  int TTL;
}Route;

typedef struct{
  uint16_t command;
  unit16_t num_entries;
  struct{
    unit32_t cost;
    unit32_t address;
  }entries[num_entries];


}RIP;

class Table{
    
private:
  map<unit32_t,Route> myTable;  
public:
  Table(Interface interface);

  void update(RIP rip);
  RIP makeReq();
  RIP makeResp(int interface_id);

};


#endif /* defined(__Table__) */
