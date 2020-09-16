
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

int main(int argc, char * argv[]) {


	char buffer [500] ;
	fstream file;
	int charsRead = 0;
	
	file.open(argv[1]);
	memset(buffer, '\0', sizeof(buffer));
	
	while(file.eof() != true){
			file.read(buffer, sizeof(buffer) - 4);
			if(file.eof()){
				strcat(buffer, " @@");
			}
			cout << buffer ;
			charsRead += strlen(buffer);
			memset(buffer, '\0', sizeof(buffer));
	}
	if(file.eof()){
		cout << "\nEnd of file reached" << endl;
	}
	cout << "\ncharsRead: " << charsRead << endl;
	file.close();
	return 0;

}
