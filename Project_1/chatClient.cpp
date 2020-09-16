/******************************************************************************
 * Project 1
 * Author: Justin Azevedo
 * Class: CS 372 Networking
 * Program: chatServe
 * Date: 5/12/19
 * Description: Client code for Project 1 re-written in C++. Program takes 
 * hostname & port # from commandline in that order.
 * Works referenced: 
 * 		CS 344 Assignment 4 Networking with C code
 * 		Beej's guide to network programing. 
 * ****************************************************************************/
#include<iostream>
#include<string>
#include<cstring>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<netinet/in.h>
#include<cstdlib>
#include<unistd.h>
#include<stdio.h>
using namespace std; 


//Function to check input for "\quit"
bool quitCheck(char * buffer){
	
	bool result; 
	char quit [] = "\\quit" ;		//Backslash is listed twice to escape itself. 
	char * found = NULL; 

	found = strstr(buffer, quit);	//Look for substring.

	if(found == NULL){				//If pointer is null no substring is found. 
		result = true;
	}
	else{
		result = false;
	}

	return result;
}


//Print error function
void error(string message){

	cout << "Error: " << message << '\n' ;
	exit(1);
}

//Initiate Contact with server
int initiateContact(char *host, char *port){
	
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	int portNumber;
	int socketFD;

	memset((char*)&serverAddress, '\0', sizeof(serverAddress));
	portNumber = atoi(port);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNumber);
	serverHostInfo = gethostbyname(host);
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length);

	socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if(socket < 0){
		error("Socket not created!");
	}

	if(connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
		error("Unable to connect to server!");
	}

	return socketFD;
	
}

//Get handle from user. 
void getHandle(char *handle){
	memset(handle, '\0', sizeof(handle));
	cout << "Please enter a user handle: " ;
	cin.getline(handle, 10);
	strcat(handle, "> ");
}

//Get keyboard input from user. 
void getMessage(char *handle, char *buffer){
	char message [488];
	memset(message, '\0', sizeof(message));	
	memset(buffer, '\0', sizeof(buffer));
	cout << handle ;
	cin.getline(message, sizeof(message));
	strcpy(buffer, handle);
	strcat(buffer, message);
//	cout << "getMessage at end of function: " << buffer << endl;

}

//Send Message function
void sendMessage(int socketFD, char * buffer){

	int charsSent;
	do{
		charsSent = send(socketFD, buffer, strlen(buffer), 0);
		if(charsSent == 0){
			error("Unable to send, socket is closed!");
		}
		else if(charsSent < 0){
			error("Send failed!");
		}
	}while(charsSent < strlen(buffer));
}


//Main Function
int main(int argc, char *argv[]){

	int socketFD;
	int charsRead;
	char buffer[501]; 						//500 for message + handle + 1 for null terminating bit
	char message[488];						//500 message - 12 handle = 488
	char handle [12];						//Handle is ten chars + two for "> " 
	char serveHandle [] = "chatServe> ";
	bool stayConnected = true;


	//Check number of commandline args
	if(argc < 3){
		error("Missing Host name or port on commandline! \n");	
		exit(1);
	}

	socketFD = initiateContact(argv[1], argv[2]);


	getHandle(handle);

	while(stayConnected){
		getMessage(handle, buffer);								//Get user input
		sendMessage(socketFD, buffer);							//Send input
		stayConnected = quitCheck(buffer);						//Check input for quit, if found, break loop close socket
		if(stayConnected == false){		
			break;
		}
		memset(buffer, '\0', sizeof(buffer));					//Clear buffer for recv
		recv(socketFD, buffer, (sizeof(buffer) - 1), 0);		//recv message from server
		cout << buffer << endl;
		stayConnected = quitCheck(buffer);						//Perform second quit check
	}

	cout << "Chat closed\n" ;									//Close connection 
	close(socketFD);


	return 0;
}			

