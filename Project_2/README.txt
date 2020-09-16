Contents: ftserver.cpp, ftclient.py, makefile

To Compile / Run

    ftserver.cpp:
        -> "make ftserver" to compile.
        -> "./ftserver <PORTNUM>" to run.
        -> "make clean" to remove executable.
    
    ftclient.py
        -> "python3 ftclient.py <SERVER_HOST> <SERVER_PORT> <COMMAND> <FILENAME> <DATA_PORT>" to run
        -> If command == '-l', <FILENAME> can be omitted. 

EXTRA CREDIT!!!

->Login functionality
This program implements a login and password for the client to gain access to the server. Upon launching ftclient.py the user is prompted for a login and password. The login and password are listed below. Incorrect login credentials will prevent ftclient from performing any file transfer functionality. Given correct credentials the program proceeds normally. 

    ->Login: admin
    ->Password: 1234
     
