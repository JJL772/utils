
BINDIR=../bin/
CC=gcc
CXX=g++
CXXFLAGS+=-g -fsanitize=address
LDFLAGS+=-ltinyxml2
SRC=$(wildcard *.cpp) 
INCDIR=../include/

all:
	mkdir -p $(BINDIR) 
	$(CXX) -o $(BINDIR)/bhc-parse $(CXXFLAGS) $(SRC) $(LDFLAGS)

.PHONY: clean install 

install:
	mkdir -p $(INCDIR)/bhc-parse
	cp -r ./*.h $(INCDIR)/bhc-parse/ 


clean:
	rm $(BINDIR)/bhc-parse 

