CXX = -g++

CXXFLAG = -pedantic-errors -Wall -Wextra -Werror

all: main
	./main.o < prueba.txt

main: 
	$(CXX) -o main.o main.cpp

clear:
	rm -rf main.o data.dat ind.dat 
