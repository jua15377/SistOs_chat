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
char userNameg[SIZE_STATUS];
char cTo[SIZE_STATUS];
char *clientTo;
char *who;
char *msgTemp;

char *mHandShake = "{\"host\":\"";
typedef struct localInfo{
    char* userName;
    int socket;
    char* id;
}Info ;
Info info;

char* LastcharDel(char* name);
void register2(message){
    struct json_object *response, *userJson;

    struct json_object  *id, *user,*status,*messagej,*from,*to,*action;
    response = json_tokener_parse(message);
    json_object_object_get_ex(response, "user", &user);
    char *idString =  json_object_get_string(user);
    userJson = json_tokener_parse(idString);
    json_object_object_get_ex(userJson, "id", &id);
    char *idString2 =  json_object_get_string(id);
    printf("%s\n", message);
    //printf("%s\n", idString);
    printf("Registro exitoso%s\n");
    printf("%s\n", idString2);
    info.id = idString2;
    
    }
    char * getId(message,alias){
  printf("%s\n",alias);
  
  struct json_object *response, *userJson,*user_obj;
  struct json_object  *id, *user,*status,*messagej,*from,*to,*action,*name;
  response = json_tokener_parse(message);
  json_object_object_get_ex(response, "users", &user);
  char *idString =  json_object_get_string(user);
  printf("--- Usuarios conectados ---\n" );
 /* printf("------%s\n" );
  printf("%s", idString);
  printf("------%s\n" );*/
  char *idString2;
  char *idString3;
  char *idString4;
  int userListSize = json_object_array_length(user);
  //printf("%d", userListSize);
  int i;
  for (i = 0; i < userListSize; ++i) {
      user_obj = json_object_array_get_idx(user, i);
      json_object_object_get_ex(user_obj, "id", &id);
    json_object_object_get_ex(user_obj, "name", &name);
    json_object_object_get_ex(user_obj, "status", &status);
    idString2 = json_object_get_string(name);
    idString3 = json_object_get_string(status);
    idString4 = json_object_get_string(id);
    printf("%s\n",idString2 );

    if (strstr(idString2,LastcharDel(alias)) != NULL) {
        printf("%s\n", idString4 );
        return  idString4;
    }
  }
  return "null";
}

// This function get the current ip of the client
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

 // This function is only for inputs like fgets
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

// Function that notifies when a new user is online
newUser(message){
struct json_object *response, *userJson;

    struct json_object  *id, *user,*status,*messagej,*from,*to,*action,*name;
    response = json_tokener_parse(message);
    json_object_object_get_ex(response, "user", &user);
    char *idString =  json_object_get_string(user);
    userJson = json_tokener_parse(idString);
    json_object_object_get_ex(userJson, "name", &name);
    char *idString2 =  json_object_get_string(name);
    //printf("%s\n", message);
    //printf("%s\n", idString);
    printf("%s", idString2);
    printf(" se acaba de conectar");
    
}
disconectUser(message){
struct json_object *response, *userJson;

    struct json_object  *id, *user,*status,*messagej,*from,*to,*action,*name;
    response = json_tokener_parse(message);
    json_object_object_get_ex(response, "user", &user);
    char *idString =  json_object_get_string(user);
    userJson = json_tokener_parse(idString);
    json_object_object_get_ex(userJson, "name", &name);
    char *idString2 =  json_object_get_string(name);
    //printf("%s\n", message);
    //printf("%s\n", idString);
    printf("%s", idString2);
    printf(" se acaba de desconectar");
}

// Function that receive a message to the user
char * getMessage(message){
  struct json_object *response, *userJson;

    struct json_object  *id, *user,*status,*messagej,*from,*to,*action,*name;
    response = json_tokener_parse(message);
    json_object_object_get_ex(response, "from", &from); // TODO Handle to use with the user
    json_object_object_get_ex(response, "message", &messagej);
    char *idString =  json_object_get_string(from);
    char *idString2 =  json_object_get_string(messagej);
    printf("message\n");
    printf("%s\n",idString2 );
    return idString2;
   // printf("%s", idString);
    //printf("> %s",idString2);
}
char * getMessageId(message){
  struct json_object *response, *userJson;

    struct json_object  *id, *user,*status,*messagej,*from,*to,*action,*name;
    response = json_tokener_parse(message);
    json_object_object_get_ex(response, "from", &from); // TODO Handle to use with the user
    json_object_object_get_ex(response, "message", &messagej);
    char *idString =  json_object_get_string(from);
    char *idString2 =  json_object_get_string(messagej);
    printf("from\n");
    printf("from %s\n",idString );
    return idString;
   // printf("%s", idString);
    //printf("> %s",idString2);
}
void receiveMessage(message){
  struct json_object *response, *userJson;

    struct json_object  *id, *user,*status,*messagej,*from,*to,*action,*name;
    response = json_tokener_parse(message);
    json_object_object_get_ex(response, "from", &from); // TODO Handle to use with the user
    json_object_object_get_ex(response, "message", &messagej);
    char *idString =  json_object_get_string(from);
    char *idString2 =  json_object_get_string(messagej);
    printf("%s", idString);
    printf("> %s",idString2);
}
receiveStatus(message){
   struct json_object *response, *userJson;
  struct json_object  *id, *user,*status,*messagej,*from,*to,*action,*name;
  response = json_tokener_parse(message);
  json_object_object_get_ex(response, "user", &user);
  char *idString =  json_object_get_string(user);
  userJson = json_tokener_parse(idString);
  json_object_object_get_ex(userJson, "name", &name);
  json_object_object_get_ex(userJson, "status", &status);
  char *idString2 =  json_object_get_string(name);
  char *idString3 =  json_object_get_string(status);
  //printf("%s\n", message);
  //printf("%s\n", idString);
  printf("%s", idString2);
  printf(" ahora esta: %s",idString3);
}
char * receiveUser2(message,who){
  //printf("mensaje%s\n");
  printf("%s\n",message);
  printf("--------------\n");
  printf("%s\n",who );
   struct json_object *response, *userJson,*user_obj;
struct json_object  *id, *user,*status,*messagej,*from,*to,*action,*name;
  response = json_tokener_parse(message);
  json_object_object_get_ex(response, "users", &user);
  char *idString =  json_object_get_string(user);
  printf("--- Usuarios conectados ---\n" );
 /* printf("------%s\n" );
  printf("%s", idString);
  printf("------%s\n" );*/
  char *idString2;
  char *idString3;
  int userListSize = json_object_array_length(user);
  //printf("%d", userListSize);
  int i;
  for (i = 0; i < userListSize; ++i) {
      user_obj = json_object_array_get_idx(user, i);
      json_object_object_get_ex(user_obj, "id", &id);
    json_object_object_get_ex(user_obj, "name", &name);
    json_object_object_get_ex(user_obj, "status", &status);
    idString2 = json_object_get_string(id);
    idString3 = json_object_get_string(status);
      if (strstr(idString2,who) != NULL) {
        
        idString2 = json_object_get_string(name);
        return idString2;
    }    
    
  }

  
}
void receiveUser(message){
  //printf("mensaje%s\n");
  printf("%s\n",message);
  printf("--------------\n");
  printf("%s\n",who );
   struct json_object *response, *userJson,*user_obj;
struct json_object  *id, *user,*status,*messagej,*from,*to,*action,*name;
  response = json_tokener_parse(message);
  json_object_object_get_ex(response, "users", &user);
  char *idString =  json_object_get_string(user);
  printf("--- Usuarios conectados ---\n" );
 /* printf("------%s\n" );
  printf("%s", idString);
  printf("------%s\n" );*/
  char *idString2;
  char *idString3;
  int userListSize = json_object_array_length(user);
  //printf("%d", userListSize);
  int i;
  for (i = 0; i < userListSize; ++i) {
      user_obj = json_object_array_get_idx(user, i);
      json_object_object_get_ex(user_obj, "id", &id);
    json_object_object_get_ex(user_obj, "name", &name);
    json_object_object_get_ex(user_obj, "status", &status);
    idString2 = json_object_get_string(id);
    idString3 = json_object_get_string(status);
      if (strstr(idString2,who) != NULL) {
        //printf("%s\n", idString4 );
        printf("Alias:" );
        idString2 = json_object_get_string(name);
    printf("%s", idString2);
    printf("  y se encuentra actualmente:" );
    printf("%s\n", idString3);
    }    
    
  }

  
}
void receiveUsers(message){
  //printf("mensaje%s\n");
  printf("%s\n",message);
  
   struct json_object *response, *userJson,*user_obj;
struct json_object  *id, *user,*status,*messagej,*from,*to,*action,*name;
  response = json_tokener_parse(message);
  json_object_object_get_ex(response, "users", &user);
  char *idString =  json_object_get_string(user);
  printf("--- Usuarios conectados ---\n" );
 /* printf("------%s\n" );
  printf("%s", idString);
  printf("------%s\n" );*/
  char *idString2;
  char *idString3;
  int userListSize = json_object_array_length(user);
  //printf("%d", userListSize);
  int i;
  for (i = 0; i < userListSize; ++i) {
      user_obj = json_object_array_get_idx(user, i);
      json_object_object_get_ex(user_obj, "id", &id);
    json_object_object_get_ex(user_obj, "name", &name);
    json_object_object_get_ex(user_obj, "status", &status);
    idString2 = json_object_get_string(name);
    idString3 = json_object_get_string(status);
    printf("Alias:" );
    printf("%s", idString2);
    printf("  y se encuentra actualmente:" );
    printf("%s\n", idString3);
  }
  //userJson = json_tokener_parse(idString);
  /*char *idString2;
  int userListSize=0;
  userListSize = json_object_array_length(userJson);
  printf("%s", userListSize);
  int i;
  for (i = 0; i < userListSize; ++i) {
      user_obj = json_object_array_get_idx(user, i);
      json_object_object_get_ex(user_obj, "id", &id);
    json_object_object_get_ex(user_obj, "name", &name);
    json_object_object_get_ex(user_obj, "status", &status);
    idString2 = json_object_get_string(id);
    printf("ids %s\n" );
    printf("%s", idString2);
  }*/
 // userJson = json_tokener_parse(idString);
  //json_object_object_get_ex(userJson, "name", &name);
  //json_object_object_get_ex(userJson, "status", &status);
  //char *idString2 =  json_object_get_string(name);
  //char *idString3 =  json_object_get_string(status);
  //printf("%s\n", message);
  //printf("%s\n", idString);
  
}

void error_handler(char * message){
  struct json_object *error, *e_message;
  error = json_tokener_parse(message);
  json_object_object_get_ex(error, "message", &e_message);
  char * error_msj = json_object_get_string(e_message);
  printf(">>ERROR<<: %s\n", error_msj);
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
              printf("\nConexion terminada\n");
              exit(1);
        } else {
          // In this case is the register
              if (strstr(message, "OK")!=NULL){
                if (strstr(message, userNameg)!=NULL){
                  register2(message);
              }
              }
              // In this case is when a new user is online
              if (strstr(message, "USER_CONNECTED")!=NULL){
                  newUser(message);
                  
              }
              // When the session finish
              if (strstr(message, "BYE")!=NULL){
                  printf("Gracias por utilizar el chat\n" );
                  close(fd);
                  exit(0);
              }
              // When the session of another user finish
              if (strstr(message, "USER_DISCONNECTED")!=NULL){
                  disconectUser(message);
              }
              if (strstr(message, "RECEIVE_MESSAGE")!=NULL){
                  //clientTo = "rec";
                  //printf("///////\n");
                  //msgTemp = message;
                 // getUsersInfo();
                  receiveMessage(message);
              }
              if (strstr(message, "CHANGED_STATUS")!=NULL){
                  receiveStatus(message);
              }
              if (strstr(message, "ERROR")!=NULL){
                  error_handler(message);
                  exit(0);
              }
              if (strstr(message, "LIST_USER")!=NULL){
                  
                  
                  if (strstr(clientTo,"null")!=NULL){
                    receiveUsers(message);
                  }
                  if (strstr(clientTo,"msg")!=NULL){
                    who = getId(message,cTo);
                    sendMessage(stat,who);
                  }
                  if (strstr(clientTo,"rec")!=NULL){
                    printf("**********\n");
                    printf("%s\n",msgTemp );
                    printf("%s\n",message );
                    char *idString =  getMessage(msgTemp);
                    char *idString2 =  getMessageId(msgTemp);
                    char *idString3 = receiveUser2(message,idString2);
                    printf("%s",idString3);
                    printf(">%s\n", idString);
                  }
                  else{
                    printf("Hasta aqui\n");
                    who = getId(message,cTo);
                    receiveUser(message,who);
                  }
                  
              }
              int comp;
              comp = strncmp(message,"BYE",2);
              printf("\nServer> %s", message);
              printf("\n");
              //printf("%s", prompt);
              printf("Ingrese una opcion:\n");
              fflush(stdout); // Make sure "User>" gets printed
          
          }
    }
}

// This function is to remove the last character of a message typed.
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

// Function to send the handShakeMessage
void handShakeMessage(userName){
  
    struct json_object *request;

    struct json_object  *host, *origin, *user;
    request = json_object_new_object();
    host = json_object_new_string(server_IP);
    printf("Local ipp is : %s \n" , getIp());
    origin = json_object_new_string(getIp());
    user = json_object_new_string(userName);

    json_object_object_add(request, "host", host);
    json_object_object_add(request, "origin", origin);
    json_object_object_add(request, "user", user);
    
    //char *respuesta1 =  json_object_get_string(request);
    //printf("%s\n","HolaHola" );
    char *respuesta1 = json_object_get_string(request);
    printf("Local ipp is : %s \n" , getIp());
    printf("%s \n",respuesta1);
    printf("\n");
    //send(fd, respuesta1, BUFFER_MSJ_SIZE, 0);
      //memset(respuesta1, 0, BUFFER_MSJ_SIZE);
    send(fd, respuesta1, BUFFER_MSJ_SIZE, 0);
    
}

// This function is to send a message to another user
sendMessage(message,clientTo){
  //printf("El mensaje a enviar es>%s\n", message);
 
 struct json_object *request, *userJson;
 struct json_object  *id, *user,*status,*messagej,*from,*to,*action,*name;
 request = json_object_new_object();
 action = json_object_new_string("SEND_MESSAGE");
 from = json_object_new_string(info.id);
 to = json_object_new_string(clientTo); //TODO handle id of other users
 messagej = json_object_new_string(LastcharDel(message));
 json_object_object_add(request, "action", action);
 json_object_object_add(request, "from", from);
 json_object_object_add(request, "to", to);
 json_object_object_add(request, "message", messagej);
 char *respuesta = json_object_get_string(request);
 printf("%s \n",respuesta);
 send(fd, respuesta, BUFFER_MSJ_SIZE, 0);
}
changeStatus(message){
  printf("%s\n", message);

 struct json_object *request, *userJson;
 struct json_object  *id, *user,*status,*messagej,*from,*to,*action,*name;
 request = json_object_new_object();
 action = json_object_new_string("CHANGE_STATUS");
 id = json_object_new_string(info.id);
 status = json_object_new_string(LastcharDel(message));
json_object_object_add(request, "action", action);
json_object_object_add(request, "user", id);
json_object_object_add(request, "status", status);
char *respuesta = json_object_get_string(request);
printf("%s \n",respuesta);
send(fd, respuesta, BUFFER_MSJ_SIZE, 0);
 
}

void getUsersInfo(){

 struct json_object *request, *userJson;
 struct json_object  *id, *user,*status,*messagej,*from,*to,*action,*name;
 request = json_object_new_object();
 action = json_object_new_string("LIST_USER");
 json_object_object_add(request, "action", action);
 char *respuesta = json_object_get_string(request);
 printf("%s \n",respuesta);
 send(fd, respuesta, BUFFER_MSJ_SIZE, 0);
}
void getUserInfo(alias){

 struct json_object *request, *userJson;
 struct json_object  *id, *user,*status,*messagej,*from,*to,*action,*name;
 request = json_object_new_object();
 action = json_object_new_string("LIST_USER");
 user= json_object_new_string(alias); //TODO
 json_object_object_add(request, "action", action);
 json_object_object_add(request, "user", user);
 char *respuesta = json_object_get_string(request);
 printf("%s \n",respuesta);
 send(fd, respuesta, BUFFER_MSJ_SIZE, 0);
}
int main(int argc, char const *argv[]){
  // It needs 4 parameters
  if (argc != 4){
    printf("Uso: client <username> <server_ip> <server_port>\n");
        exit(1);
  }
  else{
    server_IP = argv[2];
    port = (u_short) atoi(argv[3]);
    userNameg[SIZE_STATUS] = argv[1];

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

    
    info.userName = argv[1];
    info.socket = fd;
  
    handShakeMessage(info.userName);
    pthread_t thread;
    pthread_create(&thread, NULL, &recive, (void *) &info);
    printf("Conectando Servidor ...\n");
    

  while(1) {  


        printf("1.Chat con un usuario\n");
        printf("2.Cambiar estado\n");
        printf("3.Usuarios e informacion\n");
        printf("4.Informacion en especifico de un usuario \n");
        printf("5. Salir\n");
          
        opcion = atoi(scanInput());
        switch(opcion)
         {
           case 1: //  Chat with a user
           clientTo="msg";
            printf("Ingrese el alias de usuario para chatear: \n");
            fgets(cTo, 100, stdin);
            printf("Ingrese un nuevo mensaje: \n");
            fgets(stat, 100, stdin);
            getUsersInfo();
            //sendMessage(stat,clientTo);
            
             break;

           case 2: //  change status

             printf("Ingrese el nuevo estado: \n");
             printf("active\n");
             printf("busy \n");
             printf("inactive\n");
             fgets(stat, 100, stdin);
             changeStatus(stat);
             break;

           case 3: // Get users connected info
            clientTo="null";
            getUsersInfo(); //TODO put a parameter to handle only connected users

             break;

           case 4: //  get specific info of a user
           clientTo="";
              printf("Ingrese el alias de usuario para ver su informacion: \n");
              fgets(cTo, 100, stdin);
              getUsersInfo();
              //getUserInfo(clientTo);
              //getUserInfo(clientTo); // TODO 
             break;

           case 5: //BYE
             send(fd, "BYE", BUFFER_MSJ_SIZE, 0);
            
         }
      
      
      //An extra breaking condition can be added here (to terminate the while loop)
  }
}