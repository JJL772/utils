PROJECT=epics-tk
BINDIR=../bin/
CC=gcc
CXX=g++
CXXFLAGS+=-g -fsanitize=address
LDFLAGS+=-ltinyxml2
SRC=$(wildcard *.cpp) 
INCDIR=../include/

all:
	mkdir -p $(BINDIR) 
	#$(CXX) -o $(BINDIR)/$(PROJECT) $(CXXFLAGS) $(SRC) $(LDFLAGS)

.PHONY: clean install 

install:
	mkdir -p $(INCDIR)/$(PROJECT)
	cp -r ./*.hpp $(INCDIR)/$(PROJECT)/ 


clean:
	rm $(BINDIR)/$(PROJECT)

