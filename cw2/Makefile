# Generic Makefile for GP2X or PC development
#
#

#TYPE=arm or  TYPE=pc

TYPE=arm

ifeq ($(TYPE), arm)
#ARM Platform

PLATFORM=GP2X_PLATFORM
CROSS_COMPILE = /opt/open2x/gcc-4.1.1-glibc-2.3.6/bin/arm-open2x-linux-
SDL_BASE = /opt/open2x/gcc-4.1.1-glibc-2.3.6/bin/
EXT=gpe
LIBS = -lsvl -I/opt/open2x/include/ -I/opt/open2x/gcc-4.1.1-glibc-2.3.6/include/ `$(SDL_BASE)sdl-config --libs --cflags` -static
else
# PC Platform

PLATFORM=PC_PLATFORM
CROSS_COMPILE = /usr/bin/
SDL_BASE = /usr/bin/
EXT=exe
LIBS = `$(SDL_BASE)sdl-config --libs` -lsvl -lasound -ldl  -lSDL_image
endif

NAME = $(TYPE)cwk

MFILE=Makefile

#LDFLAGS = -static

CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
STRIP = $(CROSS_COMPILE)strip

CXXFLAGS =  `$(SDL_BASE)sdl-config --cflags` -Wall -D$(PLATFORM) $(LIBS) -O3
VER = 1.1


SDLCWK_TARGET = $(NAME).$(EXT)
SDLCWK_OBJS = sdlwindow.o utilities.o engine.o edgebuffer.o texture.o contextstate.o sphere.o
SDLCWK_H = sdlwindow.h utilities.h engine.h edgebuffer.h texture.h contextstate.h sphere.h
all : $(SDLCWK_TARGET)

$(SDLCWK_OBJS):	$(SDLCWK_H) $(MFILE)
main.o:	$(SDLCWK_H) $(MFILE)

#$(SDL_OBJS):	pixel.h gp2x.h game.h

CODE_LIB=libgame_$(TYPE).a

$(SDLCWK_TARGET) : main.o $(SDLCWK_OBJS)
	$(CROSS_COMPILE)ar -r  $(CODE_LIB) $(SDLCWK_OBJS)
	$(CXX) $(LDFLAGS) -o $(SDLCWK_TARGET) main.o $(CODE_LIB)  $(LIBS) -lm -O3
#	$(STRIP) $(SDLCWK_TARGET)

clean:
	rm -f $(ALL_TARGETS) *.o *~ *.a
