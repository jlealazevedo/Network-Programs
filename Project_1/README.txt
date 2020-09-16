chatClient.cpp instructions: 
	
	To compile type:
		make chatClient

	To run type: 
		chatClient [host] [port #] 

    To remove executable type: 
        make clean

chatServe.py instructions:
	
	To compile and run type: 
		python3 chatclient.py [port #]


NOTE: Testing was done using "localhost" on flip2 server. Client and server must alternate messages, code is not setup to handle asynchronous communication. To quit type \quit. chatServe will remaining running accepting new clients if chatClient stops. To stop chatServe type ctrl + c or SIGINT. 

Works cited: 
    ->Server code in c from CS344 operating systems was used as a guide for chatClient.cpp. 

    ->Course text book chapter 2.7 was reference for creating chatServe.py
    
    ->Beej's guide to network programming

    ->www.tutorialspoint.com/python
