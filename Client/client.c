#include <json.h>

#include <stdio.h>
#include <sys/socket.h> //For Sockets
#include <stdlib.h>
#include <netinet/in.h> //For the AF_INET (Address Family)
#include <string.h>
#include <pthread.h>

#define BUFFER_MSJ_SIZE 1024

struct sockaddr_in server; //This is our main socket variable.
int fd; //This is the socket file descriptor that will be used to identify the socket
int conn; //This is the connection file descriptor.
char message[BUFFER_MSJ_SIZE] = ""; //This array will store the messages that are sent by the server
char message2[BUFFER_MSJ_SIZE] = "";
char *server_IP;
u_short port;
int opcion;

typedef struct localInfo{
    char* userName;
    int socket;
}Info ;

char * create_handshake(){

}


char* scanInput()
{
  char *message = malloc(BUFFER_MSJ_SIZE );
  memset(message, 0, BUFFER_MSJ_SIZE );
  if (message == NULL){
    printf("No memory for input.\n");
    return NULL;
  }

  fgets(message, BUFFER_MSJ_SIZE , stdin);

  if((strlen(message) > 0) && (message[strlen (message) - 1] == '\n'))
  {
    message[strlen (message) - 1] = '\0';
  }

  return message;
}
/*Recive data from server HERE SHOULD HANDEL the answers from server*/
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
	if (argc != 4){
		printf("Uso: client <username> <server_ip> <server_port>\n");
        exit(1);
	}
	else{
		server_IP = argv[2];
		port = (u_short) atoi(argv[3]);

	}
	fd = socket(AF_INET, SOCK_STREAM, 0); // creates new local socket
	server.sin_family = AF_INET;// set teh type of comunication
	server.sin_port = htons(port); // set port to connect

	inet_pton(AF_INET, server_IP, &server.sin_addr); //This binds the client to the server IP

	connect(fd, (struct sockaddr *)&server, sizeof(server)); //This connects the client to the server.
	//TODO handle exceptions
opcion=8;
printf("----------------------------------------------------------\n");
    printf("------------------------Chat------------------------------\n");
    printf("----------------------------------------------------------\n");
    printf("----------------------------------------------------------\n");
    printf("----------------------------------------------------------\n");
    printf("---------------------Bienvenido---------------------------\n");
    printf("----------------------------------------------------------\n");
    printf("\n");
	//test
	Info info;
    info.userName = argv[1];
    info.socket = fd;
    pthread_t thread;
    pthread_create(&thread, NULL, &recive, (void *) &info);
    printf("Conectando Servidor...");

	while(1) {	

 printf("Ingrese una opcion:\n");
        printf("1.Chat con todos\n");
        printf("2.Chat con un usuario\n");
        printf("3.Cambiar estado\n");
        printf("4.Usuarios e informacion\n");
        printf("5.Informacion en especifico de un usuario \n");
        printf("6. Ayuda\n");
         printf("7. Salir\n");
         opcion = atoi(scanInput());
         switch(opcion)
         {
           case 1: //  Chat with all
              printf("%s","Ingrese su mensaje: " );
              fgets(message, 100, stdin);
      send(fd, message, BUFFER_MSJ_SIZE, 0);
      memset(message, 0, BUFFER_MSJ_SIZE);
      //printf(info.userName);
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