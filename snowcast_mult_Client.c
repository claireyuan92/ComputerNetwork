#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#include<pthread.h> //for threading , link with lpthread

void *connection_handler(void *);

int server(uint16_t port);
int client(const char * addr, uint16_t port);

#define MAX_MSG_LENGTH (512)//(512)
#define MAX_BACK_LOG (5)

int main(int argc, char ** argv)
{
	if (argc < 3) {
		printf("Command should be: myprog s <port> or myprog c <port> <address>\n");
		return 1;
	}
	int port = atoi(argv[2]);
	if (port < 1024 || port > 65535) {
		printf("Port number should be equal to or larger than 1024 and smaller than 65535\n");
		return 1;
	}
	if (argv[1][0] == 'c') {
		if(argv[3]==NULL){
			printf("NO IP address is given\n");
			return 1;
		}
		return client(argv[3], port);
	} else if (argv[1][0] == 's') {
		return server(port);
	} else {
		printf("unknown command type %s\nCommand should be: myprog s <port> or myprog c <port> <address>", argv[1]);
		return 1;
	}
    pthread_exit(NULL);

	return 0;
}

int client(const char * addr, uint16_t port)
{
	int sock;
	struct sockaddr_in server_addr;
	char msg[MAX_MSG_LENGTH], reply[MAX_MSG_LENGTH*3];

	if ((sock = socket(AF_INET, SOCK_STREAM/* use tcp */, 0)) < 0) {
		perror("Create socket error:");
		return 1;
	}

	printf("Socket created\n");
	server_addr.sin_addr.s_addr = inet_addr(addr);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("Connect error:");
		return 1;
	}

	printf("Connected to server %s:%d\n", addr, port);

	int recv_len = 0;
	while (1) {
		fflush(stdin);
		printf("Enter message: \n");
		gets(msg);
		if (send(sock, msg, MAX_MSG_LENGTH, 0) < 0) {
			perror("Send error:");
			return 1;
		}
		recv_len = read(sock, reply, MAX_MSG_LENGTH*3);
		if (recv_len < 0) {
			perror("Recv error:");
			return 1;
		}
		reply[recv_len] = 0;
		printf("Server reply:\n%s\n", reply);
		memset(reply, 0, sizeof(reply));
	}
	close(sock);
	return 0;
}


int server(uint16_t port)
{
	/*
		Add your code here */
		
        int s, new_s, *new_socket;
	struct sockaddr_in sin;

	//build address data structure
	bzero (&sin, sizeof (sin));//???char*
	//char buf[MAX_MSG_LENGTH];//thread
	sin.sin_family = AF_INET;
	sin.sin_port = htons (port);
	sin.sin_addr.s_addr = INADDR_ANY;//??????

	//setup passive open
	if((s=socket (AF_INET, SOCK_STREAM, 0)) < 0){
		perror("simplex-talk: socket\n");
		exit(1);
	}
	if ((bind (s, (struct sockaddr *)&sin, sizeof(sin)))<0){
		perror("simplex-talk: bind\n");
		exit(1);
	}
	listen (s, MAX_BACK_LOG);
        //new_socket = malloc(4);//???1

	//printf("listening\n");
	//wait for connection
		while(1){
	  //printf("while(1)\n");

	        socklen_t slen = sizeof (sin);
		//int len;
		if((new_s = accept (s, (struct sockaddr *) &sin, &slen))<0){
			perror("simplex-talk: accept\n");
			exit(1);
		}
		//printf("after if new_s\n");
        pthread_t sniffer_thread;
        new_socket = malloc(4);//???1
        *new_socket = new_s;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_socket) < 0)//Valgrind??
        {
            perror("error: create thread");
            return 1;
        }
         
        //????join the thread , so that we dont terminate before the thread:pthread_join( sniffer_thread , NULL);
        puts("Handler");
	pthread_detach( sniffer_thread);//!!!!!!!!!!!!!!!!!!!!!no possible memmery leak
		//	close (new_s);
	}
	
	/*socklen_t slen = sizeof (sin);
		//int len;
		while((new_s = accept (s, (struct sockaddr *) &sin, &slen))>=0){
		  //	perror("simplex-talk: accept\n");
		  //	exit(1);
		
		//printf("after if new_s\n");
        pthread_t sniffer_thread;
        new_socket = malloc(4);//???1
        *new_socket = new_s;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_socket) < 0)//Valgrind??
        {
            perror("error: create thread");
            return 1;
        }
         
        //dont terminate before the thread:pthread_join( sniffer_thread , NULL);
        puts("Handler");

		//	close (new_s);
		}*/
		//close(*new_socket);//???????useful?
	//pthread_exit(NULL);
	//free(new_socket);

	return 0;
}


 void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int new_s = *(int*)socket_desc;
   
    char buf[MAX_MSG_LENGTH+1];
    //int s, new_s;

    //	        socklen_t slen = sizeof (sin);
		int len;
     
		while ((len = recv(new_s, buf, MAX_MSG_LENGTH,0))){
		  //printf("while recv\n");
		  buf[MAX_MSG_LENGTH]='\0';
               	char msg[3*MAX_MSG_LENGTH];//thread??????????
		strcpy(msg, buf);//,strnlen(buf, MAX_MSG_LENGTH) );
		strcat(msg, buf);//,strnlen(buf, MAX_MSG_LENGTH) );
		strcat(msg, buf);
		//msg[3*MAX_MSG_LENGTH-1]='\0';

		  printf("recv from client: %s\n",buf);
			if (send(new_s, msg, strnlen(msg, 3*MAX_MSG_LENGTH-1)+1, 0) <0) {
				perror("Send error:");
		                //printf("send to client: %s once",buf);

			}
			memset(buf,0,sizeof(buf)/sizeof(char));
			memset(msg,0,sizeof(msg)/sizeof(char));
		}
     
    if(len == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(len == -1)
    {
        perror("recv failed");
    }
    //close(new_s);//??????
    //Free the socket pointer
    free(socket_desc);
    pthread_exit(NULL);//???useful?
     
    //return 0;
}
