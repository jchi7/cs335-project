# cs335 lab1
# to compile your project, type make and press enter
CFLAGS = -I ./include
LIB    = 
LFLAGS = -std=c++0x -lX11 -lGLU -lGL -lm #-lXrandr

all: game.o hero.o basicEnemy.o room.o gameObject.o platform.o spikes.o vecFxns.o collisions.o main

mac2:
	g++ main.cpp game.cpp hero.cpp basicEnemy.cpp room.cpp gameObject.cpp platform.cpp spike.cpp ppm.cpp collisions.cpp vecFxns.cpp -Wall -oGame -lX11 -lGL -lGLU -lm -I/usr/X11R6/include -L/usr/X11R6/lib -lX11 -std=c++11

315:
	g++ main.cpp game.cpp hero.cpp basicEnemy.cpp room.cpp gameObject.cpp platform.cpp spike.cpp collisions.cpp vecFxns.cpp -Wall -oGame -lX11 -lGL -lGLU -lm -I/usr/X11R6/include -L/usr/X11R6/lib -L/usr/lib/nvidia-331/ -lX11 -std=c++11

linux:
	g++ main.cpp -Wall -oGame -lX11 -lGL -lGLU -lm

main:  main.cpp
	g++ $(CFLAGS) main.cpp game.o hero.o basicEnemy.o room.o gameObject.o platform.o collisions.o vecFxns.o -Wall -Wextra $(LFLAGS) -o Game

game.o:  game.h game.cpp
	g++ -c game.cpp
gameObject.o: gameObject.h gameObject.cpp
	g++ -c gameObject.cpp
hero.o:  hero.h hero.cpp
	g++ -c hero.cpp
basicEnemy.o: basicEnemy.cpp basicEnemy.h
	g++ -c basicEnemy.cpp
room.o: room.h room.cpp
	g++ -c room.cpp
platform.o: platform.h platform.cpp
	g++ -c platform.cpp
spike.o: spike.h spike.cpp
	g++ -c spike.cpp
collisions.o: collisions.cpp collisions.h
	g++ -c collisions.cpp
vecFxns.o: vecFxns.cpp vecFxns.h
	g++ -c vecFxns.cpp

UnitTests: unitTests.h unitTests.cpp game.o gameObject.o hero.o basicEnemy.o room.o platform.o spike.o collisions.o vecFxns.o
	g++ -o UnitTests unitTests.cpp game.o gameObject.o hero.o basicEnemy.o room.o platform.o spike.o collisions.o vecFxns.o

Debug:
	g++ main.cpp -Wall -o Game -lX11 -lGL -lGLU -lm

clean:
	rm -f Game
	rm -f UnitTests
	rm -f *.o

