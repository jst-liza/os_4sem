# !/bin/bash
g++ -c 9_roots.cpp;
g++ -o 9_roots 9_roots.o -lcap;
setcap CAP_SETFCAP=+eip 9_roots;
