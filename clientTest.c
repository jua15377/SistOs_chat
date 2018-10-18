#include <json.h>

#include <stdio.h>
#include <sys/socket.h> //For Sockets
#include <stdlib.h>
#include <netinet/in.h> //For the AF_INET (Address Family)
#include <string.h>
#include <pthread.h>
#include<errno.h> //errno
#include<arpa/inet.h> //getsockname
#include<unistd.h>    //close

#define BUFFER_MSJ_SIZE 1024
#define SIZE_STATUS 20

struct sockaddr_in server; //This is our main socket variable.
int fd; //This is the socket file descriptor that will be used to identify the socket
int conn; //This is the connection file descriptor.
char message[BUFFER_MSJ_SIZE] = ""; //This array will store the messages that are sent by the server
char message2[BUFFER_MSJ_SIZE] = "";
char *server_IP;
u_short port;
int opcion;
char stat[SIZE_STATUS];

char *mHandShake = "{\"host\":\"";
typedef struct localInfo{
    char* userName;
    int socket;
}Info ;

char* LastcharDel(char* name);

/*void * sendHandShake(){
      
      
      //send(fd, message, BUFFER_MSJ_SIZE, 0);
      //memset(message, 0, BUFFER_MSJ_SIZE);
}*/
char* getIp(){

    const char* google_dns_server = "8.8.8.8";
    int dns_port = 53;
     
    struct sockaddr_in serv;
     
    int sock = socket ( AF_INET, SOCK_DGRAM, 0);
     
    //Socket could not be created
    if(sock < 0)
    {
        perror("Socket error");
    }
     
    memset( &serv, 0, sizeof(serv) );
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr( google_dns_server );
    serv.sin_port = htons( dns_port );
 
    int err = connect( sock , (const struct sockaddr*) &serv , sizeof(serv) );
     
    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    err = getsockname(sock, (struct sockaddr*) &name, &namelen);
         
    char buffer[100];
    const char* p = inet_ntop(AF_INET, &name.sin_addr, buffer, 100);
         
    if(p != NULL)
    {
        //printf("Local ip is : %s \n" , buffer);
       close(sock);
        return buffer;
    }
    else
    {
        //Some error
        printf ("Error number : %d . Error message : %s \n" , errno , strerror(errno));
    }
 
   
    
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
              int comp;
              comp = strncmp(message,"BYE",2);
              if (comp!=0){
                    printf("\nServer> %s", message);
              printf("\n");
              //printf("%s", prompt);
              printf("Ingrese una opcion:\n");
              fflush(stdout); // Make sure "User>" gets printed
          
              }
              else{

              }
          }
    }
}

char* LastcharDel(char* name)
{
    int i = 0;
    while(name[i] != '\0')
    {
        i++;
         
    }
    name[i-1] = '\0';
    return name;
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

    struct json_object *request;

    struct json_object  *host, *origin, *user;
    request = json_object_new_object();
    host = json_object_new_string(server_IP);
    printf("Local ipp is : %s \n" , getIp());
    origin = json_object_new_string(getIp());
    user = json_object_new_string(info.userName);

    json_object_object_add(request, "host", host);
    json_object_object_add(request, "origin", origin);
    json_object_object_add(request, "user", user);
    
    //char *respuesta1 =  json_object_get_string(request);
    printf("%s\n","HolaHola" );
    char *respuesta1 = json_object_get_string(request);
    printf("Local ipp is : %s \n" , getIp());
    printf("%s \n",respuesta1);
    printf("\n");
    //send(fd, respuesta1, BUFFER_MSJ_SIZE, 0);
      //memset(respuesta1, 0, BUFFER_MSJ_SIZE);

    pthread_t thread;
    pthread_create(&thread, NULL, &recive, (void *) &info);
    printf("Conectando Servidor ...\n");
    send(fd, respuesta1, BUFFER_MSJ_SIZE, 0);
    //memset(respuesta1, 0, BUFFER_MSJ_SIZE);

  while(1) {  

        printf("1.Chat con todos\n");
        printf("2.Chat con un usuario\n");
        printf("3.Cambiar estado\n");
        printf("4.Usuarios e informacion\n");
        printf("5.Informacion en especifico de un usuario \n");
         printf("6. Salir\n");
          
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
            printf("active\n");
            printf("busy \n");
            printf("inactive\n");
            fgets(stat, 100, stdin);
            printf("%s\n", stat);
             struct json_object *change;
             struct json_object  *action, *user, *status;
             change = json_object_new_object();
             action = json_object_new_string("CHANGE_STATUS");
             user = json_object_new_string(info.userName);
             status = json_object_new_string(LastcharDel(stat));
            json_object_object_add(change, "action", action);
            json_object_object_add(change, "user", user);
            json_object_object_add(change, "status", status);
            char *respuesta2 = json_object_get_string(change);
            printf("%s \n",respuesta2);
            send(fd, respuesta2, BUFFER_MSJ_SIZE, 0);


             break;

           case 4: //  get Users status
          

             break;

           case 5: // get info of users
             break;
            case 6:
            send(fd, "BYE", BUFFER_MSJ_SIZE, 0);
            printf("Gracias por utilizar el chat\n" );
            exit(0);
         }
      
      
      //An extra breaking condition can be added here (to terminate the while loop)
  }
}