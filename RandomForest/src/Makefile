CC = g++

LDFLAGS = `pkg-config --cflags --libs opencv4` -std=c++17

HH_FILES = $(shell find . -name '*.h')
CPP_FILES = $(shell find . -name '*.cc')

main: $(HH_FILES)
	$(CC) $(CPP_FILES) $(LDFLAGS)

clean:
	rm -rf a.out core *.o