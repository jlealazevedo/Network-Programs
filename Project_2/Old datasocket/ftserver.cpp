/********************************************************************************
* Author: Justin Azevedo
* Program: ftserver
* Course: CS372 Networking
* Section: 400
* Last Modified: 5-26-2019
* Program Description: Mock FTP server program. Takes port number from
* commandline to establish control socket. Accepts clients on control socket.
* Accepts two commands from client for '-l' for listing current working 
* directory or '-g <fileName>' to transfer a file from the server to the client. 
* Works Cited:
********************************************************************************/
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
#include<dirent.h>
#include<fstream>
using namespace std;

//Rec: String containing error message
//Ret: Nothing
//Pre: Valid string input
//Des: Prints error message to screen before exiting program.
void error(string message){

	cout << "ERROR: " << message << '\n' ;
	exit(1);
}

//Rec: Nothing.
//Ret: Number of files in the current working directory.
//Pre: OS considers "." as current working directory. 
//Des: Counts the number of files in the current directory. 
//It does not count the current directory "." or the parent ".." as
//valid files. 
int countFilesInDir(){

	DIR *dirPtr;
	struct dirent *dirContents;
	int numFiles = 0;

	dirPtr = opendir(".");
	if(dirPtr == NULL){
		error("opendir FAILED!");
	}

	dirContents = readdir(dirPtr);

	while(dirContents){
		
		//"." and ".." d_names have a reclen of 24. They should 
		//not be counted as transferable files. 
		if(dirContents->d_reclen != 24){
			numFiles++;
		}

		dirContents = readdir(dirPtr);
	}
	
	closedir(dirPtr);

	return numFiles;
}


//Rec: Empty array of strings set to number of files in directory.
//Ret: Nothing.
//Pre: nameArr is set to correct size and is uninitialized.
//Des: Populates passed in string array with the file names in the
//current working directory. 
void getFileNamesInDir(string nameArr []){

	DIR *dirPtr;
	struct dirent *dirContents;
	int i = 0;

	dirPtr = opendir(".");
	if(dirPtr == NULL){
		error("opendir FAILED!");
	}

	dirContents = readdir(dirPtr);

	while(dirContents){

		//"." and ".." d_names have a reclen of 24. They should 
		//not be counted as transferable files. 
		if(dirContents->d_reclen != 24){
			nameArr[i] = dirContents->d_name;
			i++;
			
		}

		dirContents = readdir(dirPtr);
	}
	closedir(dirPtr);
}


//Rec: Character pointer to port name from commandline
//Ret: Integer of socket file descriptor. 
//Pre: Valid port number is passed in. 
//Des: Sets up a command socket for ftp server. Binds and
//sets socket to listen on passed in port. 
int setupComSock(char * port){

	int comSock ;
	struct addrinfo hints; 
	struct addrinfo *servInfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC ;				//IPV4 or IPV6
	hints.ai_socktype = SOCK_STREAM;			//Use tcp connection
	hints.ai_flags = AI_PASSIVE; 				//Fill in server IP automatically

	//Populate address struct
	if(getaddrinfo(NULL, port, &hints, &servInfo) != 0){
		error("getaddrinfo failed!");
	}

	//Create socket
	comSock = socket(servInfo->ai_family, servInfo->ai_socktype, servInfo->ai_protocol);
	if(comSock < 0){
		error("Create command socket failed!");
	}

	//Bind socket
	if(bind(comSock, servInfo->ai_addr, servInfo->ai_addrlen) < 0){
		error("Bind failed!");
	}

	//Set listen to accept 1 client at a time (For now)
	if(listen(comSock, 1) < 0){
		error("Listen failed!");
	}
	else{
		cout << "Server open on port: " << port << "\n" ;
	}

	freeaddrinfo(servInfo);

	return comSock;
}

//REC: Name of the data port to connect on. Command connect file descriptor. 
//RET: Connected file descriptor for newly created data socket. 
//PRE: Data port # is correctly received by client. Command connection is 
//alive and valid. 
//DES: Creates the data socket using the data port number received by the 
//client. Connects to client on data socket and returns file descriptor. 
int setupDataSock(char * dataPortName, char * clientName){
	int dataSock; 
	int dataConnect;
	int dataPort;
	struct sockaddr_in clientAddr;
	struct hostent* clientHostInfo;
	socklen_t len;

	len = sizeof(clientAddr);
	
	memset((char *)&clientAddr, '\0', sizeof(clientAddr));

	dataPort = atoi(dataPortName);
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(dataPort);
	clientHostInfo = gethostbyname(clientName);
	memcpy((char*)&clientAddr.sin_addr.s_addr, (char*)clientHostInfo->h_addr, clientHostInfo->h_length);

	dataSock = socket(AF_INET, SOCK_STREAM, 0);
	if(dataSock < 0){
		error("Create data socket failed!");
	}

	if(connect(dataSock, (struct sockaddr*)&clientAddr, len) < 0){
		error("Connection in data socket failed!");
	}

	return dataSock;

}

//RECV: Integer for connected socket, character array. 
//RETN: Nothing
//PREC: comConnect is a valid connection to client.
//POSTC: 
//DES: Takes current connection and receives message into buffer. 
//Appends buffer to message until control characters "@@" are found. 
//Control characters are then striped off of message. 
void recvRequest(int socket, char * message1, char * message2){

	char readBuffer [100];
	int bytesRecv = 0;
	int conCodeLoc = 0;
	char * parse;
	char message [500];
	
	memset(message, '\0', (sizeof(message)));
	
	//Receive message until control code "@@" found
	while(strstr(message, "@@") == NULL){
		memset(readBuffer, '\0', sizeof(readBuffer));
		bytesRecv = recv(socket, readBuffer, (sizeof(readBuffer) - 1), 0);
		strcat(message, readBuffer);
		if(bytesRecv < 0){
			error("Receive in socket failed!");
		}
	}
	
	//Strip control code from message.
	conCodeLoc = strstr(message, "@@") - message; 
	message[conCodeLoc] = '\0';
	
	//Parse contents of message.
	parse = strtok(message, " ");

	sprintf(message1, "%s", parse);

	//Check that parse is not null, fileName may not be present
	//depending on the request.
	parse = strtok(NULL, " ");
	if(parse != NULL){
		sprintf(message2, "%s", parse);	
	}
}

//REC: Active connection on comConnect, message to send to client. 
//RET: Nothing.
//PRE: Connection is valid, message is not empty. 
//DES: Sends message to client on comConnect FD. 
void sendMessage(int socket, char * message){

	int bytesSent = 0;

	do{
		bytesSent = send(socket, message, strlen(message), 0);
		if(bytesSent < 0){
			error("Send in socket failed!");
		}
	}while(bytesSent < (strlen(message)));

}

void sendFile(char * fileName, int socket){
	
	
	char buffer [500] ;
	fstream file; 
	int bytesSent = 0;
	file.open(fileName);
	memset(buffer, '\0', sizeof(buffer));
	
	while(file.eof() != true){
			
			//Leave space for three control characters and null terminator.
			file.read(buffer, sizeof(buffer) - 4);
			if(file.eof()){
				strcat(buffer, " @@");
			}
			do{
				bytesSent = send(socket, buffer, strlen(buffer), 0); 
			}while(bytesSent < (strlen(buffer)));
		
			memset(buffer, '\0', sizeof(buffer));
	}
	
	file.close();
}

void sendFileDir(string filesInDir [], int numFiles, int dataSock){
		
		int i;
		int bytesSent; 
		char fileName [100];
		
		//Append control code to last file name. 
		filesInDir[numFiles - 1] += " @@" ;
		for(i = 0; i < numFiles ; i++){
			bytesSent = 0; 
			do{
				strcpy(fileName, filesInDir[i].c_str());
				strcat(fileName, "\n");
				bytesSent = send(dataSock, fileName, strlen(fileName), 0);
			}while(bytesSent < strlen(fileName));
		}
}

//REC: Character array.
//RET: Integer.
//PRE: Passed in character array is initialized. 
//DES: Checks command for valid input. Returns an
//integer coresponding to input type. 
int validateCommand(char command []){
	if(strcmp(command, "-g") == 0){
		return 1;
	}
	else if(strcmp(command, "-l") == 0){
		return 2;
	}
	else{
		return 0;
	}
}

bool validateLogin(char * login, char * password, char * clientLogin, char * clientPass){
	
	if((strcmp(login, clientLogin) == 0) && (strcmp(password, clientPass) == 0)){
		return true;
	}
	else{
		return false;
	}
	
}


//REC: Message from client, character arrays for command and file name.
//RET: Nothing.
//PRE: Message is populated with proper format from client.
//DES: Parses received message from client. Puts command and fileName 
//int the corresponding passed in variables. 
void parseMessage(char * message, char message1 [], char message2 []){

	char * parse;
	parse = strtok(message, " ");

	sprintf(message1, "%s", parse);

	//Check that parse is null, file name may not be present
	parse = strtok(NULL, " ");
	if(parse != NULL){
		sprintf(message2, "%s", parse);	
	}
}

//REC: Current file name wanted by client, array of directory file names, 
//number of directory files. 
//RET: Bool, true
//PRE: dirFiles and numDirFiles is populated with getFilesInDir(), message is
//populated by recvRequest
//DES: Checks that requested file from the client is in the current directory. 
//Returns true if file exists, false otherwise. 
bool checkFiles(char fileName [], string dirFiles [], int numDirFiles){

	int i = 0; 
	for(i = 0 ; i < numDirFiles ; i++){
		if(dirFiles[i] == fileName){
			return true; 
		}
	}
	return false;
}



int main (int argc, char *argv[]) {

	int numDirFiles = -1;
	int comSock;
	int dataSock;
	int comConnect;
	int comType;
	socklen_t socketSize; 
	struct sockaddr_in clientAddr;
	char message [500];
	char fileName [50];
	char command[5];
	char comPort [10];
	char dataPortName [10];
	char clientLogin [10];
	char clientPass [10];
	char clientName [1000];
	char nullInput [] = "";
	char validCom [] = "Valid Com @@";
	char invalidCom [] = "INVALID COMMAND! @@";
	char validFile [] = "Valid File Name @@";
	char invalidFile [] = "FILE NOT FOUND! @@";
	char validLogin [] = "Valid Login @@";
	char invalidLogin [] = "INVALID LOGIN OR PASSWORD! @@";
	char login [] = "admin";
	char password [] = "1234";
	bool validUser = false;
	
	
	//Get port # from commandline
	//Check number of commandline args
	if(argc < 2){
		error("Control port number not given on commandline! \n");	
		exit(1);
	}
	else{
		strcpy(comPort, argv[1]);
	}
	
	//Get number of files in directory
	numDirFiles = countFilesInDir();

	//Set file name array to number of files.
	string dirFiles [numDirFiles];

	//Populate file name array. 
	getFileNamesInDir(dirFiles);
	
	comSock = setupComSock(comPort);
	
	while(1){
		socketSize = sizeof(clientAddr);
		if((comConnect = accept(comSock, (struct sockaddr *)&clientAddr, &socketSize)) < 0 ){
			error("Accept failed!");
		}
		else{
			//Get client name
			getnameinfo((struct sockaddr *)&clientAddr, sizeof(clientAddr), clientName, sizeof(clientName), 0, 0, 0);
			cout << endl ;
			cout << "Incoming connection from "  << clientName << endl;
		}
		
		//Get login and password
		recvRequest(comConnect, clientLogin, clientPass);
		//Parse login and password
		//parseMessage(message, clientLogin, clientPass);
		
		validUser = validateLogin(login, password, clientLogin, clientPass);
		
		if(validUser == true){
			
			cout << "Valid login attempt by " << clientName << " on port " << comPort << endl;
			
			//Send confirmation of valid login
			sendMessage(comConnect, validLogin);
			
			//Receive request from client
			recvRequest(comConnect, command, fileName);

			//Parse request
			//parseMessage(message, command, fileName);

			//Validate request
			comType = validateCommand(command);
			
			//Command invalid
			if(comType == 0){
				//Invalid command send error and close 
				cout << "Invalid command requested. Sending error message to " << clientName << endl;
				sendMessage(comConnect, invalidCom);
			}
			//Command = -g
			else if(comType == 1){
				//Send command validation
				sendMessage(comConnect, validCom);
			
				//Check for requested file
				if(checkFiles(fileName, dirFiles, numDirFiles) == true){
				
					//Send file validation
					sendMessage(comConnect, validFile);
				
					//Receive data port number
					recvRequest(comConnect, dataPortName, nullInput);
					
					cout << "File requested: " << fileName << " requested on port: " << dataPortName << endl;
					
					//Establish data socket connection. 
					dataSock = setupDataSock(dataPortName, clientName);
				
					cout << "Sending " << fileName << " to " << clientName << " on port " << dataPortName << endl;
					sendFile(fileName, dataSock);
				
					close(dataSock);
				
				}
				else{
					//Send file not found error
					cout << "File " << fileName << " not found. Sending error message to " << clientName << endl;
					sendMessage(comConnect, invalidFile);
				}
				
			}
			//Command = -l
			else if(comType == 2){
			
				
				//Send command validation
				sendMessage(comConnect, validCom);
				//Recv data port # on comConnect
				recvRequest(comConnect, dataPortName, nullInput);
				
				cout << "List directory requested by " << clientName << " on port " << dataPortName << endl;
				
				//Transmit data on dataSock
				dataSock = setupDataSock(dataPortName, clientName);
				sendFileDir(dirFiles, numDirFiles, dataSock);
			
				//NOTE: Find way to include client host name and port in this message!!
				cout << "Sending directory contents to " << clientName << " on port "<< dataPortName << endl;
		
				close(dataSock);
			}
		}
		//Invalid login
		else{
			cout << "Invalid login attempt by " << clientName << " sending error message to " << clientName << endl;
			sendMessage(comConnect, invalidLogin);
		}
	}


	return 0;

}
