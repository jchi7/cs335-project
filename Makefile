# cs335 lab1
# to compile your project, type make and press enter
CFLAGS = -Wall -Wextra -I ./include
LIB    = 
LFLAGS = -std=c++0x -lX11 -lGLU -lGL -lm #-lXrandr
MAC2FLAGS = -Wall -Wextra -lalut -lopenal -lX11 -lGL -lGLU -lm -I/usr/X11R6/include -L/usr/X11R6/lib -lX11 -std=c++11 -L/System/Library/Frameworks/OpenAL.framework -I/System/Library/Frameworks/OpenAL.framework

all: game.o hero.o basicEnemy.o room.o gameObject.o platform.o spikes.o vecFxns.o collisions.o savePoint.o main

mac2:
	g++ main.cpp game.cpp fernandoV.cpp hero.cpp basicEnemy.cpp room.cpp gameObject.cpp platform.cpp savePoint.cpp markS.cpp moisesA.cpp spike.cpp ppm.cpp collisions.cpp vecFxns.cpp jasonc.cpp basicBullet.cpp -Wall -o Game $(MAC2FLAGS)

315:
	g++ main.cpp game.cpp hero.cpp basicEnemy.cpp room.cpp gameObject.cpp platform.cpp savePoint.cpp markS.cpp moisesA.cpp spike.cpp ppm.cpp jasonc.cpp collisions.cpp vecFxns.cpp basicBullet.cpp -Wall -o Game $(MAC2FLAGS) -L/usr/lib/nvidia-331/

linux:
	g++ main.cpp -Wall -o Game -lX11 -lGL -lGLU -lm

main:  main.cpp
	g++ $(CFLAGS) main.cpp game.o hero.o basicEnemy.o room.o gameObject.o platform.o collisions.o vecFxns.o savePoint.o -Wall -Wextra $(LFLAGS) -o Game

basicEnemy.o: basicEnemy.cpp basicEnemy.h
	g++ -c basicEnemy.cpp
collisions.o: collisions.cpp collisions.h
	g++ -c collisions.cpp
game.o:  game.h game.cpp
	g++ -c game.cpp
gameObject.o: gameObject.h gameObject.cpp
	g++ -c gameObject.cpp
hero.o:  hero.h hero.cpp
	g++ -c hero.cpp
platform.o: platform.h platform.cpp
	g++ -c platform.cpp
room.o: room.h room.cpp
	g++ -c room.cpp
spike.o: spike.h spike.cpp
	g++ -c spike.cpp
savePoint.o: savePoint.cpp savePoint.h
	g++ -c savePoint.cpp
vecFxns.o: vecFxns.cpp vecFxns.h
	g++ -c vecFxns.cpp

UnitTests: jasjotS.h jasjotS.cpp game.o gameObject.o hero.o basicEnemy.o room.o platform.o spike.o collisions.o vecFxns.o savePoint.o
	g++ -o UnitTests jasjotS.cpp game.o gameObject.o hero.o basicEnemy.o room.o platform.o spike.o collisions.o vecFxns.o savePoint.o $(MAC2FLAGS)

Debug:
	g++ main.cpp -Wall -o Game -lX11 -lGL -lGLU -lm

clean:
	rm -f Game
	rm -f UnitTests
	rm -f *.o

