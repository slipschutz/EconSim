#!/bin/bash


g++ -shared -fPIC PythonWrapper.cc -I/usr/include/python2.7/ -L/usr/lib/x86_64-linux-gnu/ -lboost_python-py27 -lEverything -L../lib/ -I../include/  -std=c++11 -Wl,-rpath=../lib/ -o libWrapper.so

