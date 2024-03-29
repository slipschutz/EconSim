
GOOGLE_TEST_DIR = ../googletest
GOOGLE_TEST_LIBS = -L ${GOOGLE_TEST_DIR}/build/lib -lgtest -lgmock -lgmock_main -lgtest_main -lpthread
CFLAGS=-g  -I./include -std=c++11 -I./NeuralNet -O0 -fno-lto -I${GOOGLE_TEST_DIR}/googletest/include
PYLIBFLAGS=-I./pybind11/ `python3-config --cflags --ldflags`
LIBFLAG=-Wl,-rpath=./lib
CXX=g++
EXECUTABLE=Sim
SOURCES=$(shell ls ./src/*.cc)
OBJECTS=$(SOURCES:.cc=.o) 
HEADERS=$(shell ls ./include/*.hh)

MAIN=$(addsuffix .C,$(EXECUTABLE))
MAINO=./src/$(addsuffix .o,$(EXECUTABLE))

LIBS=./lib/libEverything.so ./lib/libNeuralNet.so



.PHONY: clean get put all test sclean

all: $(EXECUTABLE) #PyWrap


$(EXECUTABLE) : $(LIBS) Sim.C
	@echo "Building $(EXECUTABLE)"
	$(CXX) $(CFLAGS) $(LIBFLAG) ./$(MAIN) $(GOOGLE_TEST_LIBS) -lEverything -lNeuralNet -L./lib/  -o $@ 
	@echo "Build succeed"


PyWrap: $(LIBS)
	g++ -shared -fPIC ./PythonWrapper/PythonWrapper.cc -Wl,-rpath=../lib $(CFLAGS) $(PYLIBFLAGS) -lEverything  -lNeuralNet -L./lib/ -I./include/  -std=c++11  -o ./PythonWrapper/PyEconSim.so


./lib/libEverything.so: $(OBJECTS)
	@echo "Building Library $@..."
	@$(CXX) $(CFLAGS) -fPIC -shared  $^ -o $@ 

./lib/libNeuralNet.so: ./NeuralNet/Network.o
	@echo "Building Library $@..."
	$(CXX) $(CFLAGS) -fPIC -shared  $^ -o $@ 

%.o : %.cc
	@echo "Compiling" $< "..."
	$(CXX) -c $(CFLAGS) $(LIBFLAGS) -fPIC $< -o $@ 


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
