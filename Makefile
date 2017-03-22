CFLAGS=-g -Wall -I./include -std=c++11 
PYLIBFLAGS=-I./pybind11/ `python-config --cflags --ldflags`
LIBFLAG=-Wl,-rpath=./lib
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
	$(CXX) $(CFLAGS) $(LIBFLAG) ./$(MAIN) -lEverything -L./lib/  -o $@ 
	@echo "Build succeed"


PyWrap: $(LIBS)
	g++ -shared -fPIC ./PythonWrapper/PythonWrapper.cc -Wl,-rpath=../lib $(CFLAGS) $(PYLIBFLAGS) -lEverything -L./lib/ -I./include/  -std=c++11  -o ./PythonWrapper/PyEconSim.so


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
