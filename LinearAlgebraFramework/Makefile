CC = g++
# -g means to enable debugger for makefile flags
CCFLAGS = -g -Wall -Wextra -std=c++2a -fsanitize=address -Wno-psabi -static-libasan
CMD = clear
all: main

INCLUDE_DIR = include
HH = $(INCLUDE_DIR)/*.h

tests = matrix_tests

main: main.cc $(HH)
	$(CC) $(CCFLAGS) main.cc

# Running each individual tests for parts of the matrices, if all works still!
test1: $(tests)/printMatrix.cc
	$(CC) $(CCFLAGS) $(tests)/printMatrix.cc

test2: $(tests)/grabMatrixVals.cc
	$(CC) $(CCFLAGS) $(tests)/grabMatrixVals.cc

test3: $(tests)/multiplyMatrix.cc
	$(CC) $(CCFLAGS) $(tests)/multiplyMatrix.cc

test4: $(tests)/squareMatrices.cc
	$(CC) $(CCFLAGS) $(tests)/squareMatrices.cc

test5: $(tests)/matricesEquals.cc
	$(CC) $(CCFLAGS) $(tests)/matricesEquals.cc

test6: $(tests)/adding_matricesToScalars.cc
	$(CC) $(CCFLAGS) $(tests)/adding_matricesToScalars.cc

test7: $(tests)/subtracting_matricesToScalars.cc
	$(CC) $(CCFLAGS) $(tests)/subtracting_matricesToScalars.cc

test8: $(tests)/multiplying_matricesToScalars.cc
	$(CC) $(CCFLAGS) $(tests)/multiplying_matricesToScalars.cc

tests: $(tests)/run_tests.cc
	$(CC) $(CCFLAGS) $(tests)/run_tests.cc

#a.out: $(tests)/run_tests.cc
#	$(CC) $(CCFLAGS) $(tests)/run_tests.cc

exp: exp3.cc
	$(CC) $(CCFLAGS) exp3.cc

clean:
	rm -f main
	rm -f a.out
	rm -rf a.out.dSYM
	$(CMD)
