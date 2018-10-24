#include <stdio.h>
#include <sys/socket.h> 
#include <stdlib.h>
#include <netinet/in.h> 
#include <string.h>
#include <pthread.h>
#include <json.h>

#define MAX_USER 10
#define BUFFER_MSJ_SIZE 8000
#define CLIENT_ADDRESS_LENGTH 100

struct sockaddr_in serv, cl_socket; //variable principal del socket del server
int fd, cl_socket_fd; //socket file descriptor idnetifica al socket
int conn; //This is the connection file descriptor that will be used to distinguish client connections.
char message[100] = ""; //This array will store the messages that are sent by the server
u_short port;
// a variable to see how many clients are connected
int clients_count = 0;
char client_address[CLIENT_ADDRESS_LENGTH];

char *algo = "{\"host\": \"192.168.1.2\",\"origin\": \"192.168.1.3\",\"user\": \"ALVARITO\"}";

// store data from clients
typedef struct cliente_info
{	
	struct sockaddr_in socket;// the clien socjet
	int fd; // file descriptor from the above socket
	int connfd; //the file descriptor generated from the new connection
	char alias[50]; // the alias/nickname asseigned to by the client
	char status[10];  // this shold save the state of the client
	char uid[5];	// an Id to identify each client
	char ip[20];
}Cliente;

// to share data between threads
struct arg_struct {
    int arg1;
    int arg2;
};

Cliente connected_clients[MAX_USER]; // this array will store all the connected client

/* THE SERVER IS CREATED */

void * start_server(){
	// preparando conexion
	printf("starting serer...\n");
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
	sendto(new_socket_fd, message, strlen(message), 0, (struct sockaddr *) &cl_addr, sizeof cl_addr);
	return;
}

void succesful_reg(int id){
	char *message;
	// constructing json
	/*
	{
  	"action": "USER_CONNECTED",
	  	"user": {
			"id": "ASdbjkxclz+asd?",
			"name": "<nombre>",
			"status": "<status>"
	 	}
	}
	**/
	struct json_object *parent, *action, *user, *the_id, *name, *status;
	parent = json_object_new_object();
	user = json_object_new_object();
	action = json_object_new_string("USER_CONNECTED");
	the_id = json_object_new_string(connected_clients[id].uid);
	printf(" alias %s\n", connected_clients[id].alias);
	name = json_object_new_string(connected_clients[id].alias);
	printf("status %s\n", connected_clients[id].status);

	status = json_object_new_string(connected_clients[id].status);
	//adding properties to response objects
	//1st param = target object to store data
	//2nd param = key to json_object
	//3rd param = json object content
	json_object_object_add(user, "id", the_id);
	json_object_object_add(user, "name", name);
	json_object_object_add(user, "status", status);
	json_object_object_add(parent, "action", action);
	json_object_object_add(parent, "user", user);
	message = json_object_get_string(parent);

	int i = 0;
    for ( i = 0; i < MAX_USER; ++i)
    {
    	if(id != i){
    		if( strcmp(connected_clients[i].status,"")!=NULL){
    			send_message(connected_clients[i].connfd, &connected_clients[i].socket, message);
    			printf("->Sending succesful new connection to: %s\n", connected_clients[i].alias);
    		}	
    	}
    }
}


//Function that creates a string in json-type
char * errorFunction(char *message){
	char buffer[32];
	struct json_object *error, *r_status, *r_message;
	error  = json_object_new_object();
	r_status = json_object_new_string("ERROR");
	r_message = json_object_new_string(message);

	json_object_object_add(error, "status", r_status);
	json_object_object_add(error, "message", r_message);

	char *prueba = json_object_get_string(error);
	return prueba;
}

void disconnect_client(int id){
	int fd;
	char *message1;
	char *message;
	/**
	{
  	"action": "USER_DISCONNECTED",
	  	"user": {
			"id": "ASdbjkxclz+asd?",
			"name": "<nombre>",
			"status": "<status>"
	 	}
	}
	*/
	//creating json
	struct json_object *parent, *action, *user, *the_id, *name, *status;
	parent = json_object_new_object();
	user = json_object_new_object();
	action = json_object_new_string("USER_DISCONNECTED");
	the_id = json_object_new_string(connected_clients[id].uid);
	name = json_object_new_string(connected_clients[id].alias);

	status = json_object_new_string(connected_clients[id].status);
	//adding properties to response objects
	//1st param = target object to store data
	//2nd param = key to json_object
	//3rd param = json object content
	json_object_object_add(user, "id", the_id);
	json_object_object_add(user, "name", name);
	json_object_object_add(user, "status", status);
	json_object_object_add(parent, "action", action);
	json_object_object_add(parent, "user", user);
	message = json_object_get_string(parent);
	// cleaning the user info..

	strcpy(connected_clients[id].status,"");
	fd = connected_clients[id].connfd;
	message1 = "BYE";
	//sending response to the client to disconect 
	send_message(connected_clients[id].connfd, &connected_clients[id].socket, message1);
	close(fd);
	
	int i = 0;
    for ( i = 0; i < MAX_USER; ++i)
    {
    	if(id != i){
    		if( strcmp(connected_clients[i].status,"")!=NULL){
    			send_message(connected_clients[i].connfd, &connected_clients[i].socket, message);
    			printf("->Sending disconect alert to: %s\n", connected_clients[i].alias);
    		}	
    	}
    }
    printf("%s has gone!... :(\n", connected_clients[id].alias);
    pthread_exit(NULL);

}

void actionHandler(char *action_request, int id){
	struct json_object *request, *rq_action, *rq_from, *rq_to, *rq_message;
	printf("action request%s\n",action_request );
	request = json_tokener_parse(action_request);

	json_object_object_get_ex(request, "action", &rq_action);

	//Obtaining action description
	char *action_string = json_object_get_string(rq_action);

	int cmp_snd_msg, cmp_list_usr = 1;
	cmp_snd_msg = strcmp(action_string, "SEND_MESSAGE");
	cmp_list_usr = strcmp(action_string, "LIST_USER");
	// printf("El resultado de este compare es = %d\n", cmp_snd_msg);
	// printf("action_string = %s\n", action_string);
	// printf("action_content = %s\n", action_content);
	if(cmp_snd_msg == 0){
		//creating server response
		struct json_object *response, *response_action;

		json_object_object_get_ex(request, "from", &rq_from);
		json_object_object_get_ex(request, "to", &rq_to);
		json_object_object_get_ex(request, "message", &rq_message);

		response = json_object_new_object();
		response_action = json_object_new_string("RECEIVE_MESSAGE");

		json_object_object_add(response, "action", response_action);
		json_object_object_add(response, "from", rq_from);
		json_object_object_add(response, "to", rq_to);
		json_object_object_add(response, "message", rq_message);

		char *from_string = json_object_get_string(rq_from);
		char *to_string = json_object_get_string(rq_to);

		
		char *handlerAnswer = json_object_get_string(response);

		printf("repuesta del send_message = \n%s\n", handlerAnswer);

		//INSERTAR ACA INSTRUCCION PARA MANDAR EL MENSAJE 
		send_message(connected_clients[id].connfd, &connected_clients[id].socket, handlerAnswer);


	}else{
		if(cmp_list_usr == 0){
			struct json_object *response, *rq_usr, *response_action, *user_list;
			response = json_object_new_object();
			json_object_object_get_ex(request, "user", &rq_usr);
			char *rq_usr_content = json_object_get_string(rq_usr);
			response_action = json_object_new_string("LIST_USER");
			json_object_object_add(response, "action", response_action);
			if(rq_usr_content == NULL){
				//user is not defined so it returns all connected users.
				
				user_list = json_object_new_array();
				int i;
				for ( i = 0; i < MAX_USER; ++i)
			    {
			    	if(strcmp(connected_clients[i].status,"")!=NULL){
    					struct json_objecet *user;
						user = json_object_new_object();

			    		struct json_object *id, *name, *status;

			    		//char *usr_id = ;
			    		id = json_object_new_string(connected_clients[i].uid);
			    		json_object_object_add(user, "id", id);

			    		// char *usr_name = ;
			    		name = json_object_new_string(connected_clients[i].alias);
			    		json_object_object_add(user, "name", name);

			    		// char *usr_status = ;
			    		status = json_object_new_string(connected_clients[i].status);
			    		json_object_object_add(user, "status", status);

			    		//add to array of user
			    		json_object_array_add(user_list, user);
    				}
			    }
			    json_object_object_add(response, "users", user_list);
			    char *handlerAnswer = json_object_get_string(response);
			    // printf("DEBUG users list %s\n",handlerAnswer );
			    send_message(connected_clients[id].connfd, &connected_clients[id].socket, handlerAnswer);
		
				
			}else{
				//User is defined so it returns an specific user.
				printf("CONTENIDO DE USER: %s\n", rq_usr_content);
				int i;
				for(i = 0; i< MAX_USER; i++){
					if(strcmp(rq_usr_content, connected_clients[i].uid)==0){
						struct json_object *user, *name, *status;
						user = json_object_new_object();

						char *usr_name = connected_clients[i].alias;
						char *usr_status = connected_clients[i].status;
						name = json_object_new_string(usr_name);
						status = json_object_new_string(usr_status);

						json_object_object_add(user, "id", rq_usr);
						json_object_object_add(user, "name", name);
						json_object_object_add(user, "status", status);

						json_object_object_add(response, "users", user);
					}
				}

				char *handlerAnswer = json_object_get_string(response);
				send_message(connected_clients[id].connfd, &connected_clients[id].socket, handlerAnswer);
			}
		}
	}

	//action change status
	if(strcmp(action_string, "CHANGE_STATUS")==0){


		struct json_object *request_user_id, *request_status;

		//Obtaining all data from request.
		json_object_object_get_ex(request, "user", &request_user_id);
		char *request_user_id_string = json_object_get_string(request_user_id);
		json_object_object_get_ex(request, "status", &request_status);

		printf("voy por aca\n");
		struct json_object *response, *response_action, *response_user, *response_user_id, *response_user_name, *response_user_status;
		//Creating response
		response_action = json_object_new_string("CHANGE_STATUS");
		json_object_object_add(response, "action", response_action);


		//creating user value
		int i = 0;
		for(i = 0; i< MAX_USER; i++){
			if(strcmp(request_user_id_string, connected_clients[i].uid)==0){
				response_user = json_object_new_object();

				char *usr_name = connected_clients[i].alias;
				char *usr_status = connected_clients[i].status;
				response_user_name = json_object_new_string(usr_name);
				response_user_status = json_object_new_string(usr_status);

				json_object_object_add(response_user, "id", response_user);
				json_object_object_add(response_user, "name", response_user_name);
				json_object_object_add(response_user, "status", response_user_status);

				json_object_object_add(response, "users", response_user);
			}
		}

		char *handlerAnswer = json_object_get_string(response);
		send_message(connected_clients[id].connfd, &connected_clients[id].socket, handlerAnswer);
		
	}


}


struct json_object *  handshakeHandler(char *client_request, int id){
	//Creating json objects that will receive the data
	struct json_object *handshake, *client_rq_host, *client_rq_origin, *client_rq_user;
	struct json_object *response;
	//Parsing client request string to json object
	handshake = json_tokener_parse(client_request);

	//Obtaining json components and adding it to other json component
	//1st param = source json object
	//2nd param = key value to match json property
	//third param = target json object to store data
	json_object_object_get_ex(handshake, "host", &client_rq_host);
	json_object_object_get_ex(handshake, "origin", &client_rq_origin);
	json_object_object_get_ex(handshake, "user", &client_rq_user);

	//Get the string value of a json object 
	const char *username =	json_object_get_string(client_rq_user);
	const char *origin_ip =	json_object_get_string(client_rq_user);

	if(username != NULL){
		//do validation

		int i=0; //for cycle iterator
		int exists = 0; //boolean variable to get if a user exists

		//for cycle to check if user exists
		for ( i = 0; i < MAX_USER; ++i)
	    {
	    	char *name = connected_clients[i].alias;
	    	if(name == username){
	    		exists = 1;
	    	}
	    	//printf("conn dentro de for %d\n",connected_clients[i].connfd);
	    }

	    if(exists == 1){
	    	//User already exists
	    	//Initializing response object properties
	    	struct json_object  *status, *message;
	    	response = json_object_new_object();
	    	status = json_object_new_string("ERROR");
	    	message = json_object_new_string("Username already taken!");
	    	//adding properties to response objects
	    	//1st param = target object to store data
	    	//2nd param = key to json_object
	    	//3rd param = json object content
	    	json_object_object_add(response, "status", status);
			json_object_object_add(response, "message", message);

			return response;
	    }else{

	    	//User aproved
	    	struct json_object *user, *user_id, *user_name, *user_status, *status;
	    	//initializing user object properties
	    	user = json_object_new_object();
	    	user_id = json_object_new_string(connected_clients[id].uid); //insert id ger
	    	user_name = json_object_new_string(username);
	    	user_status = json_object_new_string("active");

	    	//sprintf(connected_clients[id].alias, "%s", username);
	    	strcpy(connected_clients[id].alias, username);
	    	//sprintf(connected_clients[id].status, "%s", "active");
	    	strcpy(connected_clients[id].status, "active");
	    	//adding properties to user object

	    	json_object_object_add(user, "id", user_id);
			json_object_object_add(user, "name", user_name);
			json_object_object_add(user, "status", user_status);

			//Initializing response object properties
	    	response = json_object_new_object();
	    	status = json_object_new_string("OK");

	    	//adding properties to response object
	    	json_object_object_add(response, "status", status);
	    	json_object_object_add(response, "user", user);
	    	
	    	return response;
	    }
	}
	// deprecated
	// else{
	// 	//Username is empty
	// 	//Initializing response object properties
 //    	struct json_object *message, *status;
 //    	response = json_object_new_object();
 //    	status = json_object_new_string("ERROR");
 //    	message = json_object_new_string("Username is empty!");
 //    	//adding properties to response objects
 //    	json_object_object_add(response, "status", status);
	// 	json_object_object_add(response, "message", message);
	// 	return response;
	// }
}

/*Recibe message*/
void * recive(void * arguments ) {
	//printf("thread\n");
	struct arg_struct *arg = arguments;
	//printf("thread 2\n");
 
    int socket_fd, response, id;
    char *message[BUFFER_MSJ_SIZE];
    memset(message, 0, BUFFER_MSJ_SIZE); // Clear message buffer
    //printf("thread 3\n");
    socket_fd = (int) arg->arg1;
    id = (int) arg->arg2;
    // Print received message
    while(1) {
        response = recvfrom(socket_fd, message, BUFFER_MSJ_SIZE, 0, NULL, NULL);
        if (response) {
           //json_object *res = handshakeHandler(message, id);
           //ress contains hanshakeHandler response 
           //char *ress = json_object_get_string(res);
           //send_message(connected_clients[id].connfd, &connected_clients[id].socket, ress);

           if (strstr(message, "origin")!=NULL){
            		json_object *respuesta = handshakeHandler(message, id);
            		char *resp = json_object_get_string(respuesta);
            		send_message(connected_clients[id].connfd, &connected_clients[id].socket, resp);
					printf("%s is connected\n", connected_clients[id].alias);
					succesful_reg(id);
            }
            else if(strstr(message, "action")!=NULL){
             		// handle acctions'
             		printf("action \n");
             		actionHandler(message,  id);
           

            }
            else if(strstr(message, "BYE") != NULL){
            		printf("%s requestes to quit\n", connected_clients[id].alias);
            		disconnect_client(id);
            }

     //        // byr
     //        if (strcmp(message, 'BYE')==0){
     //        	// TODO: handle close conntion
     //        	printf("Bye... \n");
     //        }
     //        else{
     //        	// handshake
     //        	printf("thread 6 handshake %d\n");

     //        	if (strstr(message, "origin"!=NULL)){
     //        		struct json_object *respuesta = handshakeHandler(message, id);
     //        		const char *resp = json_object_get_string(respuesta);
     //        		send_message(connected_clients[id].connfd, &connected_clients[id].socket, resp);
					// printf("%s",resp); 
     //        	}
     //        	else if(strstr(message, "acction")!=NULL){
     //        		// handle acctions'
     //        		printf("acction \n");

     //        	}
     //        	else{
     //        		send_message(connected_clients[id].connfd, &connected_clients[id].socket, message);
     //        		printf("else... \n");
     //        	}
     //       }
        	
        }
    }
}


/* FUNCION MAIN*/
int main(int argc, char const *argv[]){


	//testeando el action handlr
	//char *prb_send_message = "{\"action\": \"SEND_MESSAGE\",\"from\": \"sebas\",\"to\": \"jonny\",\"message\": \"Hola jonny\"}";
	//char *prb_list_usr = "{\"action\": \"LIST_USER\"}";
	//char *prb_list_usr2 = "{\"action\": \"LIST_USER\",\"user\": \"<id_del_usuario>\"}";
	//actionHandler(prb_send_message);
	//actionHandler(prb_list_usr);
	//actionHandler(prb_list_usr2);

	//parameter
	// struct json_object *respuesta = handshakeHandler(algo, 5);
	// const char *respuesta1 =	json_object_get_string(respuesta);
	pthread_t thread;
	struct arg_struct arguments;

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
	printf("waiting for new connections...\n");
	listen(fd,MAX_USER); 

	// handle conexions
	while(1) {
		//printf(" cl count %d\n", clients_count);
		conn = accept(fd, (struct sockaddr *)&cl_socket, &cl_socket_fd);	
    	if(conn < 0){
    		printf("error on creating a new connection\n");
    	}
		printf(" cl count %d\n", clients_count);
		inet_ntop(AF_INET, &(cl_socket.sin_addr), client_address, CLIENT_ADDRESS_LENGTH);
		printf("%s is connected.\n", client_address);
		connected_clients[clients_count].socket = cl_socket;
		connected_clients[clients_count].fd = cl_socket_fd;
		connected_clients[clients_count].connfd = conn;
		sprintf(connected_clients[clients_count].uid, "%d", clients_count);


		//connected_clients[clients_count].uid = clients_count;
		printf("-> users connected: %d\n", clients_count);
		//the conn fd and coun aka id is sent to the new thread
		arguments.arg1 = conn;
		arguments.arg2 = clients_count;
		clients_count ++;
		pthread_create(&thread, NULL, &recive, (void *) &arguments);

		
	}
	return 0;
}
