#include <stdio.h>
#include <sys/socket.h> //For Sockets
#include <stdlib.h>
#include <netinet/in.h> //For the AF_INET (Address Family)
#include <string.h>
#include <pthread.h>

#define BUFFER_MSJ_SIZE 1024

struct sockaddr_in server; //This is our main socket variable.
int fd; //This is the socket file descriptor that will be used to identify the socket
int conn; //This is the connection file descriptor that will be used to distinguish client connections.
char message[BUFFER_MSJ_SIZE] = ""; //This array will store the messages that are sent by the server
char message2[BUFFER_MSJ_SIZE] = "";
char *server_IP;
u_short port;


typedef struct localInfo{
    char* userName;
    int socket;
}Info ;

/*Recive data from server*/
void * recive(void * threadData) {
    int socket_fd, response;
    char message[BUFFER_MSJ_SIZE];
    Info* pData = (Info*)threadData;
    socket_fd = pData->socket;
    char* prompt = pData->userName;
    memset(message, 0, BUFFER_MSJ_SIZE); // Clear message buffer

    // Print received message
    while(1) {
        response = recvfrom(socket_fd, message, BUFFER_MSJ_SIZE, 0, NULL, NULL);
        if (response == -1) {
        	// to be implemented
          //fprintf(stderr, "recv() failed: %s\n", strerror(errno));
          break;
        } else if (response == 0) {
              printf("\nPeer disconnected\n");
              break;
        } else {
              printf("\nServer> %s", message);
              printf("%s", prompt);
              fflush(stdout); // Make sure "User>" gets printed
          }
    }
}


int main(int argc, char const *argv[]){
	// defina local variables

	// manejo de ingreso de variables
	if (argc > 5){
		printf("Uso: client <username> <client_port> <server_ip> <server_port>\n");
        exit(1);
	}
	else{
		server_IP = argv[1];
		port = (u_short) atoi(argv[2]);
	}
	fd = socket(AF_INET, SOCK_STREAM, 0); // creates new socket
	// this should be organized
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	inet_pton(AF_INET, server_IP, &server.sin_addr); //This binds the client to the server IP

	connect(fd, (struct sockaddr *)&server, sizeof(server)); //This connects the client to the server.
	//TODO handle exceptions

	//test
	Info info;
    info.userName = "you";
    info.socket = fd;
    pthread_t thread;
    pthread_create(&thread, NULL, &recive, (void *) &info);
	while(1) {	
	    printf("Enter a message: ");
	    fgets(message, 100, stdin);
	    send(fd, message, BUFFER_MSJ_SIZE, 0);
	    memset(message, 0, BUFFER_MSJ_SIZE);
	    
	    //An extra breaking condition can be added here (to terminate the while loop)
	}
}