CC=g++
# CPPFLAGS = -O3 -std=c++17 -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/bridges-cxx-3.3.0/include
CPPFLAGS = -O3 -std=c++2a
LIBS = -lcurl
LDFLAGS = -L/usr/local/curl763/lib -fsanitize=address

a.out: main.cc tile.h
	$(CC) $(CPPFLAGS) main.cc $(LDFLAGS) $(LIBS)

clean:
	rm -rf *.o a.out core
