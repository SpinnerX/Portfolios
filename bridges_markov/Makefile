CC=g++
CCFLAGS = -O3 -std=c++2a -fsanitize=address -fsanitize=undefined -static-libasan
#CCFLAGS = -O3 -std=c++17 -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/bridges-cxx-3.3.0/include
LIBS = -lcurl

a.out: main.cc
	$(CC) $(CCFLAGS) main.cc

clean:
	rm -rf *.o a.out core
