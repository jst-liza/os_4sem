# !/bin/bash
g++ -c pipe.cpp;
g++ -o pipe pipe.o -lpthread -lev;
