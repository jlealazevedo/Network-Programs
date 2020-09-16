'''
* Author: Justin Azevedo
* Program: ftclient
* Course: CS372 Networking
* Section: 400
* Last Modified: 6-2-2019
* Program Description: Mock FTP client program. Takes the following commandline
* parameters <Server Host>, <Server Port>, <Command>, <File Name>, <Data Port>.
* Connects to server on <server host> and <server port>. <Command> can be '-l' for 
* listing the directory contents of server or '-g <File Name>' for file transfer. 
* <Data port> sets the port number for the second tcp connection for data transfer.
* EXTRA CREDIT!!: This program uses a login and password functionality to connect
* to the server. Upon successful execution the user is prompted for login and password. 
* File transfer and directory listing will not proceed without correct credentials. 
*  	Login is: admin 
*	Password: 1234   
* Works Cited:
* 	*https://docs.python.org/release/3.7.3/reference/index.html
* 	*https://www.tutorialspoint.com/python/
'''
#!/usr/bin/python
import string
import sys
import os
import getpass
from socket import *


#RECV: String for error message.
#RETN: Nothing.
#PREC: Valid string input.
#POSTC: Program exits after printing error message.
#DESC: Error handling function. Prints 
#passed in error and exits program.
def error(str):
	print("ERROR: ", str)
	exit(1)


#RECV: Host name, command port, command, file name, and dataPort variables.
#RETN: Parsed values for each variable from commandline. 
#PREC: Valid passed in variables
#POSTC: Correct commandline arguments are parsed into corresponding variables. 
#DESC: Checks commandline for proper number of inputs, parses inputs to variables 
#if inputs are found. 
def parseCommLine(host, comPort, command, fileName, dataPort):
	
	if(len(sys.argv) < 5):
		error("Incorrect number of commandline args!")
	#-l command, no filename
	elif(len(sys.argv) == 5):
		host = sys.argv[1]
		comPort = sys.argv[2]
		command = sys.argv[3]
		dataPort = sys.argv[4]
	#-g command, include filename
	elif(len(sys.argv) == 6):
		host = sys.argv[1]
		comPort = sys.argv[2]
		command = sys.argv[3]
		fileName = sys.argv[4]
		dataPort = sys.argv[5]
	return host, comPort, command, fileName, dataPort

#RECV: Host name and command port number.
#RETN: File descriptor of command socket. 
#PREC: Host and command port number are valid and populated.
#POSTC: Returned socket is connected to server.
#DESC: Establish command connection with server. 
def initiateContact(host, comPort):
	comSock = socket(AF_INET, SOCK_STREAM)
	comSock.connect((host, int(comPort)))
	return comSock		

#RECV: Dataport number.
#RETN: A data socket listeing on the passed in port.
#PREC: Valid port # is passed in
#POSTC: Data port is initialized and listing for server.
#DESC: Sets up the data socket for receiving data from 
#the server. 
def setupDataSocket(dataPort):
	dataPort = int(dataPort)
	dataSock = socket(AF_INET, SOCK_STREAM)
	dataSock.bind(('',dataPort))
	dataSock.listen(1)
	return dataSock

#RECV: Initialized socket connection, strings for command and file names
#RETN: Nothing
#PREC: comSock is initialized and connected. Command and file names are parsed
#from commandline. 
#POSTC: Request prepared and sent over socket to server. 
#DES: Takes the parsed command and fileNames, appends a control character and
#sends the message to the server. 
def makeRequest(socket, message1, message2):
	message = message1 + " " + message2 + " " + "@@"
	bytes_sent = 0
	while bytes_sent < len(message):
		sent = socket.send(message.encode())
		if sent == 0:
			error("Send failed")
		bytes_sent += sent

#RECV: Initialized command socket connection. 
#RETN: Response from server. 
#PREC: Comsock is a valid connection to server. 
#POSTC: Response message is returned from server. 
#DESC: Accepts response from server over command socket. Looks for control characters
#in response and strips them off before returning the server response. 
def recvData(socket):
	conCode = "@@"
	recvBuff = ""
	message = ""
	response = ""
	while message.find(conCode) == -1:
		recvBuff = socket.recv(500)
		message += recvBuff.decode()
	conCodeLoc = message.find(conCode)
	response = message[0:conCodeLoc - 1]
	return response

#RECV: String 
#RETN: Int
#PREC: String is not Null
#POSTC: Correct integer is returned depending on response type.
#DESC: Checks server response for a validation message. Returns 1 
#for a valid login, 2 for a valid command, 3 for a valid file name
#or 0 otherwise. 
def valComResponse(response):
	if (response == "Valid Login"):
		return 1
	elif (response == "Valid Com"):
		return 2
	elif (response == "Valid File Name"):
		return 3
	else:
		return 0


#RECV: String for the parsed file name from the commandline.
#RETN: True or false.
#PREC: Valid file name is passed in.
#POSTC: Correct bool is returned depending on conditions. 
#DES: Returns true if the user chooses to overwrite an existing file
#in the current directory, or the file is not found. Returns false if a
#duplicate is found and the user declines to overwrite the file. 
def checkDupFile(fileName):

	choice = ""
	validInput = False
	curDir = os.getcwd()
	filePath = curDir + "/" + fileName
	dupFound = os.path.isfile(filePath)
	if(dupFound == True):
		print("File:", fileName, "already exists")
		print("Do you want to overwrite it?")
		while(validInput == False):
			choice = input("Enter Y for yes, n for no: ")
			if(choice == "Y"):
				validInput = True
				return True
			elif(choice == "n"):
				validInput = True
				return False
	elif(dupFound == False):
		return True

#Main

#Parse commandline contents
host, comPort, command, fileName, dataPort, response = "", "", "", "", "", ""
host, comPort, command, fileName, dataPort = parseCommLine(host, comPort, command, fileName, dataPort)

#Establish command connection with server
comSock = -1
dataSock = -1
dataConnect = -1
serveAddr = ""
saveFile = False
login = ""
password = ""
validLogin = 0

#Get login creditals
login = input("Login: ")
password = getpass.getpass()

#Establish contact
comSock = initiateContact(host, comPort)

#Send login creditals
makeRequest(comSock, login, password)

#Get login response from server
response = recvData(comSock)

#Validate response
validLogin = valComResponse(response)

#If invalid login
if(validLogin == 0):
	print("ERROR:", host, comPort, "says:\n", response)
	
#If login is valid
elif(validLogin == 1):
	
	#Make request
	makeRequest(comSock, command, fileName)

	#Receive response to request
	response = recvData(comSock)

	#Validate response
	commandValid = valComResponse(response)
	
	#If invalid command sent	
	if(commandValid == 0):
		print("ERROR:", host, comPort, "says:\n", response)
	#If valid command sent
	elif(commandValid == 2):
		
		#Get file from directory
		if(command == "-g"):	
			#wait for okay on file name from server
			response = recvData(comSock);
			
			#Validate file name response from server
			fileValid = valComResponse(response);

			#If file name exists on server
			if(fileValid == 3):
				
				#Send dataport number to server.
				makeRequest(comSock, dataPort, "")
				
				#Setup data socket and listen
				dataSock = setupDataSocket(dataPort)
				dataConnect, serveAddr = dataSock.accept()
		
				print("Receiving file:", fileName, "from:", host, ":", comPort)
				
				#recv contents of file
				fileContent = recvData(dataConnect)
				
				print("File transfer complete")
				
				#Check for file name duplicates
				saveFile = checkDupFile(fileName)
				
				if(saveFile == True):
					#Save contents of file
					 newFile = open(fileName, "w")
					 newFile.write(fileContent)
					 print("File saved")
					 dataConnect.close()
				else:
					print("File not saved")
						
			#No file on server
			else:
				print("ERROR:", host, comPort, "says:\n", response)
		
		#Get directory list
		elif(command == "-l"):
			print("Receiving directory structure from:", host, ":", comPort)
			
			#Send data port number to server	
			makeRequest(comSock, dataPort, "")
			
			#Setup data socket and listen.
			dataSock = setupDataSocket(dataPort)
			dataConnect, serveAddr = dataSock.accept()
				
			#recv contents of directory
			fileNames = recvData(dataConnect)
			print(fileNames)
			dataConnect.close()
			
comSock.close()

