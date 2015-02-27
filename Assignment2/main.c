#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#define MTU (11200)

struct interface{
    //UDP socket
    struct sockaddr_in sin;
    //IP addresses/ports
    int remote_port;
    char * my_VIP;
    char * remote_VIP;
};

struct node{
    char * host_IP;
    int my_port;
    struct interface * node_interfaces;
};

void ifconfig(){
    
}

void routes(){
}

bool down(int interface_id){
    return true;
}

bool up(int interface_id){
    return true;
}

// defined as mysend to avoid confliction with existing function send
bool mysend(const char * addr,char * msg){
    return true;
    
}

int main (int argc, char **argv){
    
    if(argc!=2){
        printf("Usage: Take one file as commandline input\n");
        return 1;
    }
    FILE *f = fopen("/Users/Meng/Google Drive/Xcodepractice/Assignment2/AInput","r");
    if(f==NULL){
        perror("Cannot open file ");
        return 1;
    }
    
    /*
     int sock;
     struct sockaddr_in server_addr;
     if((sock = socket(AF_INET, SOCK_STREAM,0))<0){
     perror("Create socket error");
     return 1;
     }
     printf("Socket created\n");
     */
    
    // Create NODE
    struct node mynode;
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    
    //1. assigning port
    if (getline(&line, &linecap, f)){
        char *p =strchr(line, ':');
        p++;
        mynode.my_port=atoi(p);
    }
    
    //2. creating interfaces
    
    char * pch;
    
    while (getline(&line,&linecap, f)>0){
        
        pch = strtok (line, ":");
        mynode.host_IP=pch;
        
        pch = strtok (NULL, " ");
        mynode.node_interfaces=malloc(sizeof(struct interface));
        mynode.node_interfaces->remote_port=atoi(pch);
        
        pch= strtok (NULL, " ");
        mynode.node_interfaces->my_VIP = pch;
        
        pch= strtok (NULL, " ");
        mynode.node_interfaces->remote_VIP = pch;
        
        mynode.node_interfaces++;
    }
    
    

    
    
    
    
    
}
