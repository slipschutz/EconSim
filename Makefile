CFLAGS=-g -Wall -I./include -std=c++11 
PYLIBFLAGS=-L/usr/lib/x86_64-linux-gnu/ -lboost_python-py27 -I/usr/include/python2.7/

CXX=g++
EXECUTABLE=Sim
SOURCES=$(shell ls ./src/*.cc)
OBJECTS=$(SOURCES:.cc=.o) 
HEADERS=$(shell ls ./include/*.hh)

MAIN=$(addsuffix .C,$(EXECUTABLE))
MAINO=./src/$(addsuffix .o,$(EXECUTABLE))

LIBS=./lib/libEverything.so

.PHONY: clean get put all test sclean

all: $(EXECUTABLE) PyWrap


$(EXECUTABLE) : $(LIBS) Sim.C
	@echo "Building $(EXECUTABLE)"
	$(CXX) $(CFLAGS) ./$(MAIN) -lEverything -L./lib/  -Wl,-rpath=./lib/ -o $@ 
	@echo "Build succeed"

PyWrap: $(LIBS)
	g++ -shared -fPIC ./PythonWrapper/PythonWrapper.cc $(CFLAGS) $(PYLIBFLAGS) -lEverything -L./lib/ -I./include/  -std=c++11 -Wl,-rpath=../lib/ -o ./PythonWrapper/libWrapper.so


./lib/lib%.so: $(OBJECTS)
	@echo "Building Library $@..."
	@$(CXX) $(CFLAGS) -fPIC -shared  $^ -o $@ 

%.o : %.cc
	@echo "Compiling" $< "..."
	@$(CXX) -c $(CFLAGS) $(LIBFLAGS) -fPIC $< -o $@ 


test:
	@echo $(LIBRARY) : $(DICTOBJ) $(OBJECTS)
	echo $(EXTRAOBJS)
	echo $(FOO)


clean:
	-rm -f $(OBJECTS)
	-rm -f $(MAINO)
	-rm -f $(EXECUTABLE)
	-rm -f $(LIBS)

sclean:
	-rm *~
	make clean
