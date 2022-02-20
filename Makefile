.PHONY: all

all:
	gcc -O2 -I/opt/netronome/include -L/opt/netronome/lib map_symbol.c -o map_symbol -lnfp -Wall
