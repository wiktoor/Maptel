CC = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -O2

maptel: maptel.o 
maptel.o: maptel.cc 

clean:
	rm -f *.o maptel