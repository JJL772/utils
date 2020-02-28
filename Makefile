
CXXFLAGS+=-I../include/ --std=c++17
CFLAGS+=-I../include/

all: includes compile_commands  
	make -C bhc-parse all CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)"
	make -C epics-tk all CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)"

install: includes
	make -C bhc-parse install CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)"
	make -C epics-tk install CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)"

clean:
	make -C bhc-parse clean CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)"
	make -C epics-tk clean CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)"

includes:
	mkdir -p include/common
	cp common/*.h include/common/ 

compile_commands:
	echo '$(CFLAGS)' > compile_flags.txt
	echo '$(CXXFLAGS)' > compile_flags.txt
