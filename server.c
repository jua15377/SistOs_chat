#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

struct sockaddr_in serv; //variable principal de los socket
int fd; //socket file descriptor idnetifica al socket
int conn; //This is the connection file descriptor that will be used to distinguish client connections.
char message[100] = ""; //This array will store the messages that are sent by the server
u_short port;

void * start_server(){
	// preparando conexion
	printf("Waiting for request...\n");
	serv.sin_family = AF_INET; //protocolo de red
	serv.sin_port = htons(port); //puesto a escuchar
	serv.sin_addr.s_addr = INADDR_ANY;//ip locar

	// crea un nuevo socket
	fd = socket(AF_INET, SOCK_STREAM, 0); //This will create a new socket and also return the identifier of the socket into fd.
	// TODO: handle errors, you can add an if condition that checks whether fd is greater than 0. If it isn't, prompt an error
	// asigna ip a socket
	bind(fd, (struct sockaddr *)&serv, sizeof(serv));
}

int main(int argc, char const *argv[]){
	if (argc < 2 | argc > 2 ){
		printf("Error: no se indico el numero de puerto\n");
		exit(0);
	}
	else{
		port = (u_short) atoi(argv[1]);
	}

	start_server();

	// escucha conexiones, segundo atributo maximo de conexiones
	printf("esperando conexion\n");
	listen(fd,5);

	// manejando conexiones
	while(conn = accept(fd, (struct sockaddr *)NULL, NULL)) {
    	pid_t pid;
    	pid = fork();
    	if(pid == 0) {
	        while (recv(conn, message, 100, 0)>0) {
	            printf("Message Received: %s\n", message);
	            //An extra breaking condition can be added here (to terminate the child process)
	            strcpy(message,"");
        	}
        	exit(0);
    	}
	}
	return 0;
}
