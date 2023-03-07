a.out: main.cc hash.h
	g++ -Wall -Wextra -Wpedantic -pedantic-errors -Wno-unused-parameter -std=c++17 -g -O2 -fmax-errors=1 -D_GLIBCXX_DEBUG -fsanitize=undefined -fsanitize=address main.cc

clean:
	rm -f a.out core *.o
