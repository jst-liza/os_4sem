# !/bin/bash
g++ -c 4p.cpp;
g++ -o 4p 4p.o -lpthread;
g++ -c 4d.cpp;
g++ -o 4d 4d.o -lpthread;
