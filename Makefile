CC=g++
CCFLAGS= -std=c++11 -fPIC -Iinclude/
AR=ar
ARFLAGS=rvs
ARTARGET=libgetopt++.a
ARSOURCE= src/libparser.o src/libcommand.o

command:
	$(CC) $(CCFLAGS) -c src/$@.cpp -o src/lib$@.o

parser:
	$(CC) $(CCFLAGS) -c src/$@.cpp -o src/lib$@.o

.PHONY: uninstall clean install install_headers

install_headers:
	cp -r include/ /usr/include/getopt++/

install: install_headers command parser
	$(AR) $(ARFLAGS) $(ARTARGET) $(ARSOURCE)
	mv $(ARTARGET) /usr/lib/$(ARTARGET)

uninstall:
	rm /usr/lib/$(ARTARGET)
	rm -r /usr/include/getopt++

clean:
	rm src/*.a src/*.o

