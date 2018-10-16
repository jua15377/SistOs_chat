#include <stdio.h>
#include <sys/socket.h> 
#include <stdlib.h>
#include <netinet/in.h> 
#include <string.h>
#include "pthread.h"

#define MAX_USER 10
#define BUFFER_MSJ_SIZE 1024
#define CLIENT_ADDRESS_LENGTH 100

struct sockaddr_in serv, cl_socket; //variable principal del socket del server
int fd, cl_socket_fd; //socket file descriptor idnetifica al socket
int conn; //This is the connection file descriptor that will be used to distinguish client connections.
char message[100] = ""; //This array will store the messages that are sent by the server
u_short port;
// a variable to see how many clients are connected
int clients_count = 0;
char client_address[CLIENT_ADDRESS_LENGTH];

// store data from clients
struct cliente
{	
	struct sockaddr_in socket;// the clien socjet
	int fd; // file descriptor from the above socket
	int connfd; //the file descriptor generated from the new connection
	int enable; // the status of the client
	char alias[50]; // the alias/nickname asseigned to by the client
	int status;  // this shold save the state of the client
	int uid;	// an Id to identify each client
};

struct cliente connected_clients[MAX_USER]; // this array will store all the connected client

/* SE CREA EL SERVIDOR */

void * start_server(){
	// preparando conexion
	printf("Preparando servidor...\n");
	serv.sin_family = AF_INET; //protocolo de red
	serv.sin_port = htons(port); //puesto a escuchar
	serv.sin_addr.s_addr = INADDR_ANY;//ip locar
	// crea un nuevo socket
	fd = socket(AF_INET, SOCK_STREAM, 0); //This will create a new socket and also return the identifier of the socket into fd.
	// TODO: handle errors, you can add an if condition that checks whether fd is greater than 0. If it isn't, prompt an error
	// asigna ip a socket
	bind(fd, (struct sockaddr *)&serv, sizeof(serv)); 
}


/*THIS FUNCIONT SEND A MESSAGE USING THE FD FROM TEH CONNETTION ANT THE CLIENT ADDR
* NEEDS MORE WORK
*/
void * send_message(int new_socket_fd, struct sockaddr *cl_addr, void * message) {
	sendto(new_socket_fd, message, 100, 0, (struct sockaddr *) &cl_addr, sizeof cl_addr);
}

/*Recibe message*/
void * recive(void * socket) {
    int socket_fd, response;
    char message[BUFFER_MSJ_SIZE];
    memset(message, 0, BUFFER_MSJ_SIZE); // Clear message buffer
    socket_fd = (int) socket;

    // Print received message
    while(1) {
        response = recvfrom(socket_fd, message, BUFFER_MSJ_SIZE, 0, NULL, NULL);
        if (response) {
            printf("%s", message);
        	int i = 0;
            for ( i = 0; i < MAX_USER; ++i)
            {
            	send_message(connected_clients[i].connfd, &connected_clients[i].socket, message);
            	printf("conn dentro de for %d\n",connected_clients[i].connfd);
            }
        }
    }
}

/* FUNCION MAIN*/
int main(int argc, char const *argv[]){
	//test pthread
	pthread_t thread;

	// Get port number
	if (argc < 2 | argc > 2 ){
		printf("Error: no se indico el numero de puerto\n");
		exit(0);
	}
	else{
		port = (u_short) atoi(argv[1]);
	}
	start_server(); // calls the function to setup the server

	// prepare the socket to listen for new connections
	printf("espreando conexion\n");
	listen(fd,MAX_USER); 

	// manejando conexiones
	while(1) {
		printf(" cl count %d\n", clients_count);
		conn = accept(fd, (struct sockaddr *)&cl_socket, &cl_socket_fd);	
    	if(conn < 0){
    		printf("error on creating a new connection\n");
    	}
    	inet_ntop(AF_INET, &(cl_socket.sin_addr), client_address, CLIENT_ADDRESS_LENGTH);
    	printf("Connectado: %s\n", client_address);

    	// pid_t pid;
    	// pid = fork();
    	//if(pid == 0) {
		connected_clients[clients_count].socket = cl_socket;
		connected_clients[clients_count].fd = cl_socket_fd;
		connected_clients[clients_count].connfd = conn;
		clients_count ++;
		printf(" cl count increment %d\n", clients_count);
		pthread_create(&thread, NULL, &recive, (void *) conn);
		// printf("came back\n");
        // while (recv(conn, message, 100, 0)>0) {
        //     // strcpy(message,"");
        //     printf("Message Received: %s\n", message);
        //     //An extra breaking condition can be added here (to terminate the child process)            
        //     // printf("pid %d\n", pid);
        //     printf("conn fd %d\n", conn);
    	// }
	}
	return 0;
}