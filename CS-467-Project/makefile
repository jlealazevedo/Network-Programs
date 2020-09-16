
CFLAGS = -g -Wall -std=c++11

mainLoopTest: GameState.o Room.o Item.o mainLoopTest.cpp parser.o 
	g++ $(CFLAGS) GameState.o Room.o Item.o mainLoopTest.cpp parser.o -o mainLoopTest

mainTest: GameState.o Room.o Item.o mainTest.o parser.o
	g++ $(CFLAGS) GameState.o Room.o Item.o mainTest.o parser.o -o mainTest

mainTest.o: mainTest.cpp GameState.hpp Item.hpp Room.hpp 
	g++ $(CFLAGS) -c mainTest.cpp GameState.hpp Item.hpp Room.hpp 

GameState.o: GameState.hpp GameState.cpp Item.hpp Room.hpp 
	g++ $(CFLAGS) -c GameState.hpp GameState.cpp Item.hpp Room.hpp 

Room.o: Room.hpp Room.cpp Item.hpp
	g++ $(CFLAGS) -c Room.hpp Room.cpp Item.hpp

Item.o: Item.hpp Item.cpp
	g++ $(CFLAGS) -c Item.hpp Item.cpp
	

parser.o: parser.hpp parser.cpp
	g++ $(CFLAGS) -c parser.hpp parser.cpp
	
clean: 
	rm *.o *.gch mainTest
