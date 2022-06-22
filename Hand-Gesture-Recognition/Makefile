CC = g++
# CCFLAGS = -Wall -Wextra --std=c++2a -I/opt/homebrew/Cellar/opencv/4.5.4_3/include/opencv4
# CCFLAGS = --std=c++2a -I/opt/homebrew/include/opencv4

# LDFLAGS = -I/opt/homebrew/include/opencv4
LDFLAGS = `pkg-config --cflags --libs opencv4` -std=c++17

HH_FILES = $(shell find . -name '*.h')
CPP_FILES = $(shell find . -name '*.cc')

main: $(HH_FILES)
	$(CC) $(CPP_FILES) $(LDFLAGS)

clean:
	rm -rf a.out core *.o