#include <stdio.h>
#include <sys/socket.h> //For Sockets
#include <stdlib.h>
#include <netinet/in.h> //For the AF_INET (Address Family)
#include <string.h>

struct sockaddr_in server; //This is our main socket variable.
int fd; //This is the socket file descriptor that will be used to identify the socket
int conn; //This is the connection file descriptor that will be used to distinguish client connections.
char message[100] = ""; //This array will store the messages that are sent by the server
char message2[100] = "";
char *server_IP;
u_short port;




int main(int argc, char const *argv[]){
	// manejo de ingreso de variables
	if (argc < 2){
		printf("Error: no se indico la ip y el numero de puerto\n");
		exit(0);
	}
	else if(argc > 3){
		printf("Demasiados argumentos\n");
		exit(0);
	}
	else{
		server_IP = argv[1];
		port = (u_short) atoi(argv[2]);
	}


	fd = socket(AF_INET, SOCK_STREAM, 0);

	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	inet_pton(AF_INET, server_IP, &server.sin_addr); //This binds the client to localhost

	connect(fd, (struct sockaddr *)&server, sizeof(server)); //This connects the client to the server.


	while(1) {
	    printf("Enter a message: ");
	    fgets(message, 100, stdin);
	    send(fd, message, strlen(message), 0);
	    recv(fd, message2, 100, 0);
	     printf("Message Received: %s\n", message2);
	    //An extra breaking condition can be added here (to terminate the while loop)
	}
}