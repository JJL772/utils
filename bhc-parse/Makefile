
BINDIR=../bin/
CC=gcc
CXX=g++
CXXFLAGS+=-g -fsanitize=address --std=c++17
SRC=$(wildcard *.cpp) 
INCDIR=../include/

all:
	mkdir -p $(BINDIR) 
	$(CXX) -o $(BINDIR)/bhc-parse $(CXXFLAGS) $(SRC) $(LDFLAGS) -ltinyxml2

.PHONY: clean install 

install:
	mkdir -p $(INCDIR)/bhc-parse
	cp -r ./*.h $(INCDIR)/bhc-parse/ 


clean:
	rm $(BINDIR)/bhc-parse 

