# cs335 lab1
# to compile your project, type make and press enter
WFLAGS = #-Wall -Wextra
CFLAGS = -I ./include
LIB    = 
LFLAGS = -std=c++0x -lX11 -lGLU -lGL -lm #-lXrandr
MAC2FLAGS = -lX11 -lGL -lalut -lopenal -lGLU -lm -I/usr/X11R6/include -L/usr/X11R6/lib -lX11 -std=c++11

all: main

mac:
	g++ -DISMAC=1 -DNVIDIA=1 main.cpp game.cpp hero.cpp basicEnemy.cpp room.cpp fernandoV.cpp gameObject.cpp platform.cpp elevator.cpp savePoint.cpp markS.cpp moisesA.cpp spike.cpp ppm.cpp collisions.cpp vecFxns.cpp jasonC.cpp basicBullet.cpp shooterEnemy.cpp spawnPoint.cpp $(WFLAGS) -o Game $(MAC2FLAGS) log.cpp fonts.cpp fonttex.cpp -pthread

315:
	g++ -DNVIDIA=1 main.cpp game.cpp hero.cpp basicEnemy.cpp room.cpp fernandoV.cpp gameObject.cpp platform.cpp elevator.cpp savePoint.cpp markS.cpp moisesA.cpp spike.cpp ppm.cpp collisions.cpp vecFxns.cpp jasonC.cpp basicBullet.cpp shooterEnemy.cpp spawnPoint.cpp $(WFLAGS) -o Game $(MAC2FLAGS) log.cpp fonts.cpp fonttex.cpp -pthread -L/usr/lib/nvidia-331/

main:
	g++ main.cpp game.cpp hero.cpp basicEnemy.cpp room.cpp fernandoV.cpp gameObject.cpp platform.cpp elevator.cpp savePoint.cpp markS.cpp moisesA.cpp spike.cpp ppm.cpp collisions.cpp vecFxns.cpp jasonC.cpp basicBullet.cpp shooterEnemy.cpp spawnPoint.cpp $(WFLAGS) -o Game $(MAC2FLAGS) log.cpp fonts.cpp fonttex.cpp -pthread

Jason:  main.cpp
	g++ $(CFLAGS) main.cpp game.o hero.o basicEnemy.o room.o gameObject.o platform.o elevator.o collisions.o vecFxns.o savePoint.o $(WFLAGS) $(LFLAGS) -o Game

basicEnemy.o: basicEnemy.cpp basicEnemy.h
	g++ -c basicEnemy.cpp
collisions.o: collisions.cpp collisions.h
	g++ -c collisions.cpp
elevator.o: elevator.cpp elevator.h
	g++ -c elevator.cpp
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

UnitTests: jasjotS.h jasjotS.cpp game.o gameObject.o hero.o basicEnemy.o room.o platform.o spike.o collisions.o elevators.o vecFxns.o savePoint.o
	g++ -o UnitTests jasjotS.cpp game.o gameObject.o hero.o basicEnemy.o room.o platform.o spike.o collisions.o elevators.o vecFxns.o savePoint.o $(WFLAGS) $(MAC2FLAGS)

Debug:
	g++ main.cpp -Wall -o Game -lX11 -lGL -lGLU -lm

clean:
	rm -f Game
	rm -f UnitTests
	rm -f *.o

