#include <stdio.h>
#include <sys/socket.h> //For Sockets
#include <stdlib.h>
#include <netinet/in.h> //For the AF_INET (Address Family)
#include <string.h>
#define BUFFER_SIZE 1024

char buffer[BUFFER_SIZE];
int sockfd = 0;
char s[INET_ADDRSTRLEN];
char *Usuario;

struct sockaddr_in server; //This is our main socket variable.
int fd; //This is the socket file descriptor that will be used to identify the socket
int conn; //This is the connection file descriptor that will be used to distinguish client connections.
char message[100] = ""; //This array will store the messages that are sent by the server
char *server_IP;
u_short port;
int opcion;

char* scanInput()
{
  char *message = malloc(BUFFER_SIZE);
  memset(message, 0, BUFFER_SIZE);
  if (message == NULL){
    printf("No memory for input.\n");
    return NULL;
  }
  
  fgets(message, BUFFER_SIZE, stdin);

  if((strlen(message) > 0) && (message[strlen (message) - 1] == '\n'))
  {
    message[strlen (message) - 1] = '\0';
  }

  return message;
}
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
	  opcion=8;
	  printf("----------------------------------------------------------\n");
	  printf("------------------------Chat------------------------------\n");
	  printf("----------------------------------------------------------\n");
	  printf("----------------------------------------------------------\n");
	  printf("----------------------------------------------------------\n");
	  printf("---------------------Bienvenido---------------------------\n");
	  printf("----------------------------------------------------------\n");
	  printf("\n");

	while(opcion != 0) {
	      printf("Ingrese una opcion:\n");
		    printf("1.Chat un usuario\n");
		    printf("2.Chat con todos\n");
		    printf("3.Cambiar estado\n");
		    printf("4.Usuarios e informacion\n");
		    printf("5.Informacion en especifico de un usuario \n");
		    printf("6. Ayuda\n");
				 printf("7. Salir\n");
				 opcion = atoi(scanInput());
				 switch(opcion)
		     {
		       case 1: //  Chat with all
		          printf("Ingrese un mensaje: ");
	    				fgets(message, 100, stdin);
	    				send(fd, message, strlen(message), 0);

		         break;

		       case 2: //  chat with a user

		         printf("Ingrese el usuario al que enviara el mensaje: \n");

		         break;

		       case 3: // Change status
		        printf("Ingrese el nuevo estado: \n");
        		printf("0 -> active\n");
        		printf("1 -> busy \n");
        		printf("2 -> inactive\n");
        		printf("Seleccione una opcion valida:\n");
		         break;

		       case 4: //  get Users status



		         break;

		       case 5: // Close connection
		         break;
						case 6:
						printf("Ayuda\n" );
						case 7:
						exit(0);
		     }
	    //An extra breaking condition can be added here (to terminate the while loop)
	}
}
