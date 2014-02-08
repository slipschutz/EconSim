CFLAGS=-c -g -Wall -I./src -I./include 
CXX=g++
EXECUTABLE=EconSim
SOURCES=$(shell ls ./src/*.cc)
OBJECTS=$(SOURCES:.cc=.o) 
HEADERS=$(shell ls ./include/*.hh)

MAIN=$(addsuffix .C,$(EXECUTABLE))
MAINO=./src/$(addsuffix .o,$(EXECUTABLE))

.PHONY: clean get put all test sclean

all: $(EXECUTABLE) 

$(EXECUTABLE) : $(MAINO) $(OBJECT) $(HEADERS)
	@echo "Building $(EXECUTABLE)"
	@$(CXX) -o $@ $^
	@echo "Build succeed"

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
	-rm -f $(DICTOBJ) $(OBJECT) $(EXTRAOBJS)
	-rm -f *Dictionary.h
	-rm -f *Dictionary.cc
	-rm -f $(LIBRARY)
	-rm -f *Parsed.cc
	-rm -f *Parsed.o
sclean:
	-rm *~
	make clean