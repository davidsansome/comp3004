.SUFFIXES: $(SUFFIXES) .o .cpp

CXX_FLAGS = -g -Wall -Werror
O_FILES = utilities.o cone.o sphere.o engine.o sdlglwindow.o main.o drawable.o

cw1: $(O_FILES)
	g++ -o cw1 $(O_FILES) -lGL -lSDL -lGLU

utilities.o: utilities.cpp utilities.h
	g++ -c utilities.cpp $(CXX_FLAGS)

drawable.o: drawable.cpp drawable.h
	g++ -c drawable.cpp $(CXX_FLAGS)

cone.o: cone.cpp cone.h utilities.o types.h
	g++ -c cone.cpp $(CXX_FLAGS)

sphere.o: sphere.cpp sphere.h utilities.o types.h
	g++ -c sphere.cpp $(CXX_FLAGS)

engine.o: engine.cpp engine.h sdlglwindow.o sphere.o utilities.o
	g++ -c engine.cpp $(CXX_FLAGS)

sdlglwindow.o: sdlglwindow.cpp sdlglwindow.h
	g++ -c sdlglwindow.cpp $(CXX_FLAGS)

main.o: main.cpp engine.h
	g++ -c main.cpp $(CXX_FLAGS)

all: cw1

clean:
	rm -fv *.o *~ cw1
