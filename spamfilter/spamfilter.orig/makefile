all: filter nn

filter: filter.c
	cc -xO5 -o filter filter.c

nn: NeuralSrc/*.cpp
	cd NeuralSrc && (g++ -std=c++11 -o ../nn *.cpp)

clean:
	rm *.o
