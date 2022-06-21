CC = g++
# Just for reference for the -I//Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/bridges-cxx-3.3.0/include
# this -I to the filepath means to link the library to the project in the Makefile, which is what allows us to use that specific library.
# CCFLAGS = -Wall -Wextra -std=c++2a -g -Wfatal-errors -I//Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/bridges-cxx-3.3.0/include
CCFLAGS = -Wall -Wextra --std=c++2a -g -Wfatal-errors
LIBS = -lcurl

a.out: main.o andrew.o
	$(CC) $(CCFLAGS) main.o andrew.o $(LIBS)

main.o: main.cc loc.h
	$(CC) $(CCFLAGS) -c main.cc

andrew.o: andrew.cc loc.h
	$(CC) $(CCFLAGS) -c andrew.cc

clean:
	rm -rf a.out *.o core
