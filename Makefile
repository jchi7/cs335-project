# cs335 lab1
# to compile your project, type make and press enter

all: mac

mac:
	g++ main.cpp InitializeLevels.cpp -Wall -oGame -lX11 -lGL -lGLU -lm -I/usr/X11R6/include -L/usr/X11R6/lib -lX11

linux:
	g++ main.cpp InitializeLevels.cpp -Wall -oGame -lX11 -lGL -lGLU -lm

clean:
	rm -f Game
	rm -f *.o

