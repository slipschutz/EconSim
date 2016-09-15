CFLAGS=-c -g -Wall -I./src -I./include -std=c++11
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


$(EXECUTABLE) : $(MAINO) $(LIBS)
	@echo "Building $(EXECUTABLE)"
	$(CXX) -g -std=c++11 -o $@ $^ 
	@echo "Build succeed"


./lib/lib%.so: $(OBJECTS)
	@echo "Building Library $@..."
	@$(CXX) -fPIC -shared  $^ -o $@ 

%.o : %.cc
	@echo "Compiling" $< "..."
	@$(CXX) $(CFLAGS) -fPIC $< -o $@ 
$(MAINO) : $(MAIN)
	@echo "Compiling" $< "..."
	@$(CXX) $(CFLAGS) -fPIC $< -o $@ 

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
