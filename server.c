#include <stdio.h>
#include <sys/socket.h> 
#include <stdlib.h>
#include <netinet/in.h> 
#include <string.h>

#define MAX_USER 10
#define BUFFER_MSJ_SIZE 1024

struct sockaddr_in serv, cl_socket; //variable principal del socket del server
int fd, cl_socket_fd; //socket file descriptor idnetifica al socket
int conn; //This is the connection file descriptor that will be used to distinguish client connections.
char message[100] = ""; //This array will store the messages that are sent by the server
u_short port;

int clients_count = 0;


// store data from clients
struct cliente
{	
	struct sockaddr_in socket;
	int fd;
	int connfd;
	int enable;
	char alias[50];
	int status;
	int uid;
};

struct cliente connected_clients[MAX_USER];

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

void * send_message(int new_socket_fd, struct sockaddr *cl_addr) {
	sendto(new_socket_fd, message, 100, 0, (struct sockaddr *) &cl_addr, sizeof cl_addr);
}

/* FUNCION MAIN*/
int main(int argc, char const *argv[]){
	// ontiene parametos de ejecucion
	if (argc < 2 | argc > 2 ){
		printf("Error: no se indico el numero de puerto\n");
		exit(0);
	}
	else{
		port = (u_short) atoi(argv[1]);
	}
	
	// se cargan configruacione sy se crea el socket
	start_server();

	// escucha conexiones, segundo atributo maximo de conexiones
	printf("espreando conexion\n");
	listen(fd,MAX_USER); 

	// manejando conexiones
	while(1) {
		printf(" cl count %d\n", clients_count);
		conn = accept(fd, (struct sockaddr *)&cl_socket, &cl_socket_fd);
    	
    	pid_t pid;
    	pid = fork();
    	if(pid == 0) {
    		connected_clients[clients_count].socket = cl_socket;
    		connected_clients[clients_count].fd = cl_socket_fd;
    		connected_clients[clients_count].connfd = conn;
    		clients_count ++;
    		printf(" cl count increment %d\n", clients_count);
	        while (recv(conn, message, 100, 0)>0) {
	            // strcpy(message,"");
	            printf("Message Received: %s\n", message);
	            //An extra breaking condition can be added here (to terminate the child process)            
	            printf("pid %d\n", pid);
	            printf("conn fd %d\n", conn);
	            // test
	            int i = 0;
	            for ( i = 0; i < MAX_USER; ++i)
	            {
	            	send_message(connected_clients[i].connfd, &connected_clients[i].socket);
	            	printf("conn dentro de for %d\n",connected_clients[i].connfd);
	            }
	            // send_message(conn, &cl_socket);
        	}
        	exit(0);
    	}	
	}
	return 0;
}