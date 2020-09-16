/********************************************************************************
* Author: Justin Azevedo
* Program: ftserver
* Course: CS372 Networking
* Section: 400
* Last Modified: 6-2-2019
* Program Description: Mock FTP server program. Takes port number from
* commandline to establish control socket. Accepts clients on control socket.
* Accepts two commands from client for '-l' for listing current working 
* directory or '-g <fileName>' to transfer a file from the server to the client. 
* EXTRA CREDIT!!: This program uses a login and password functionality. Client
* must enter proper credentials to access ftserver. 
*  	Login is: admin 
*	Password: 1234  
* Works Cited: 
* 	*https://beejs.us/guide/bgnet
* 	*www.cplusplus.com
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

//RECV: String containing error message
//RETN: Nothing
//PREC: Valid string input
//POSTC: Program prints error message and exits. 
//DESC: Prints error message to screen before exiting program.
void error(string message){

	cout << "ERROR: " << message << '\n' ;
	exit(1);
}

//RECV: Nothing.
//RETN: Number of files in the current working directory.
//PREC: OS considers "." as current working directory. 
//POSTC: Number of files in current working directory is returned.
//DESC: Counts the number of files in the current directory. 
//It does not count the current directory "." or the parent ".." as
//valid files. 
int countDirFiles(){

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


//RECV: Empty array of strings set to number of files in directory.
//RETN: Nothing.
//PREC: nameArr is set to correct size and is uninitialized.
//POSTC: nameArr is populated with all the file names of the current directory
//DESC: Populates passed in string array with the file names in the
//current working directory. 
void getDirFileNames(string nameArr []){

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


//RECV: Character pointer to port name from commandline.
//RETN: Integer of socket file descriptor. 
//PREC: Valid port number. 
//POSTC: Returned integer points to a valid listening network socket.
//DESC: Sets up a command socket for ftp server. Binds and
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

//RECV: Character pointers for dataPort and client host name. 
//RETN: Connected file descriptor for newly created data socket. 
//PREC: Parameters are initialized with valid port number and client host name. 
//port number must be free to connect on.
//POSTC: Returned integer reflects a valid data socket connection to client.
//DESC: Creates the data socket using the data port number and client host name
//Connects to client on data socket and returns file descriptor. 
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

//RECV: Integer for connected socket, two pointers to character arrays.  
//RETN: Nothing.
//PREC: comConnect is a valid connection to client. 
//POSTC: Message1 and message2 are set to parsed contents of the received 
//message.
//DES: Takes current connection and receives message into buffer. 
//Receives until control characters "@@" are found. Control characters are 
//then striped off of message. Message is then parsed and the first and 
//second substrings are passed into message1 and message2 respectively. 
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

//RECV: Active connection on comConnect, message to send to client. 
//RETN: Nothing.
//PREC: Connection is valid, message is not empty. 
//POSTC: Message is sent to client on socket. 
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

//RECV: Name of file to send and socket connection.
//RETN: Nothing.
//PREC: Socket is a valid connection, fileName is present in
//the current directory. The file is of type ".txt".
//POSTC: Complete contents of file are sent to client. 
//DESC: Opens the file at fileName. Reads file contents and sends
//over socket until end of file is reached. Control code is then 
//appended and the file is closed. Used in "-g" command request.
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

//RECV: String array of file names, integer for number of files in directory, 
//and integer for socket connection. 
//RETN: Nothing. 
//PREC: File names string array is initialized with getDirFileNames().
//numFiles is initialized with countDirFiles(). Socket is a valid connection.
//POSTC: List of current files in the directory is sent to the client. 
//DESC: Function sends list of current directory to client, used in the "-l"
//command request. 
void sendFileDir(string filesInDir [], int numFiles, int socket){
		
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
				bytesSent = send(socket, fileName, strlen(fileName), 0);
			}while(bytesSent < strlen(fileName));
		}
}

//RECV: Character array representing .
//RETN: Integer, value depended on contents of character array.
//PREC: Passed in character array is received from client. 
//POSTC: Correct integer value corresponding to type of command.
//DESC: Checks command for valid input and determines command type.
//Returns an integer coresponding to command type. 
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

//RECV: Character array pointers for correct login and password, and client input
//login and password. 
//RETN: Bool, true for valid login, otherwise false. 
//PREC: Server has received client credentials in command socket. 
//POSTC: Given valid login credentials, client is granted access to FTP service, otherwise
//client is denied access. 
//DESC: Function validates login credentials. 
bool validateLogin(char * login, char * password, char * clientLogin, char * clientPass){
	
	if((strcmp(login, clientLogin) == 0) && (strcmp(password, clientPass) == 0)){
		return true;
	}
	else{
		return false;
	}
	
}


//RECV: Current file name wanted by client, array of directory file names, 
//number of directory files. 
//RETN: Bool 
//PREC: dirFiles and numDirFiles is populated with getDirFileNames() and countDirFiles()
//respectively, fileName is parsed and populated by recvRequest().
//POSTC: True is returned if file name is found in directory, else false. 
//DESC: Checks that requested file from the client is in the current directory. 
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
	numDirFiles = countDirFiles();

	//Set file name array to number of files.
	string dirFiles [numDirFiles];

	//Populate file name array. 
	getDirFileNames(dirFiles);
	
	//Setup command socket. 
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
		
		validUser = validateLogin(login, password, clientLogin, clientPass);
		
		if(validUser == true){
			
			cout << "Valid login attempt by " << clientName << " on port " << comPort << endl;
			
			//Send confirmation of valid login
			sendMessage(comConnect, validLogin);
			
			//Receive request from client
			recvRequest(comConnect, command, fileName);

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
