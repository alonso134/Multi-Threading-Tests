CPPFLAGS=-std=c++11 -g

mthread: hw03.o
	   g++ -pthread -lpthread hw03.o -o mthread

hw03.o: hw03.cpp
		g++ $(CPPFLAGS) -c hw03.cpp

clean:
		rm *.o -f