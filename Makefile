CFLAGS=-g -Wall -I./src -I./include -std=c++11 -I/usr/include/python2.7/
LIBFLAGS=-L/usr/lib/x86_64-linux-gnu/ -lboost_python-py27
CXX=g++
EXECUTABLE=Sim
SOURCES=$(shell ls ./src/*.cc)
OBJECTS=$(SOURCES:.cc=.o) 
HEADERS=$(shell ls ./include/*.hh)

MAIN=$(addsuffix .C,$(EXECUTABLE))
MAINO=./src/$(addsuffix .o,$(EXECUTABLE))

LIBS=./lib/libEverything.so

.PHONY: clean get put all test sclean

all: $(EXECUTABLE) 


$(EXECUTABLE) : $(LIBS)
	@echo "Building $(EXECUTABLE)"
	@$(CXX) $(CFLAGS) -fPIC -lEverything -L./lib/ ./$(MAIN) -Wl,-rpath=./lib/ -o $@ 
	@echo "Build succeed"


./lib/lib%.so: $(OBJECTS)
	@echo "Building Library $@..."
	@$(CXX) -fPIC -shared  $^ -o $@ 

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
