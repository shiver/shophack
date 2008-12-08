VER = 0.12

all:
	g++ items.cpp ui.cpp main.cpp -lcurses -o shophack

build:
	tar czf shophack-${VER}.tar.gz *.cpp *.h *.def Makefile README

clean:
	rm shophack
