'''
Project 1
Author: Justin Azevedo
Class: CS 372 Networking
Program: chatClient
Date: 5/12/19
Description: Server code for project 1 re-written in python. 
program takes port # from commandline.
Works Referenced: 
    *Course text book chapter 2.7 on python server code.
    *www.tutorialspoint.com/python
'''


#!/usr/bin/python
import string
import sys
from socket import *

#Function for printing error messages.
def error(str):
	print("Error: ", str)
	return; 

#Function for Startup
def startup(port):
	serverSocket = socket(AF_INET, SOCK_STREAM)
	serverSocket.bind(('', port))
	serverSocket.listen(1)
	print("Server now listening on port: ", port)
	return serverSocket

#Send message to client
def send(message, serverSocket):
	bytes_sent = 0
	while bytes_sent < len(message):
		sent = serverSocket.send(message.encode())
		if sent == 0:
			error("Unable to send socket is closed!")
		bytes_sent = bytes_sent + sent

	
	
#Receive message from client
def receive(serverSocket):
	response = serverSocket.recv(500)
	print(response.decode())
	return response


#Get keyboard input from user. 
def getInput(handle):
	message = input(handle)
	fullMessage = handle + message
	return fullMessage

#Check for \quit in string
def quitCheck(message):
		noQuit = message.find("\\quit")
		if noQuit != -1:
			return False
		else:
			return True


#Check command line for correct number of inputs.  
if (len(sys.argv) < 2) : 
	error("Incorrect number of command line arguements!")
	exit()

handle = "chatServe> "
port = int(sys.argv[1])

#Pass commandline port number to startup 
socketFD = startup(port)

#Infinite server loop
while(1):

	connection, addr = socketFD.accept()
	print("Accepting client on port ", port)
	keepAlive = True

	while(keepAlive == True):
		
		response = receive(connection)
		keepAlive = quitCheck(response.decode())
		if(keepAlive == False):
				break
		message = getInput(handle)
		send(message, connection)
		keepAlive = quitCheck(message)
		

	print("Chat closed")
	connection.close()


