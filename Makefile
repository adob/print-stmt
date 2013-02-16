all: test example

run-tests: test
	./test && echo "all tests passed"

test: test.cpp print.h
	c++ -O2 -Wall -o test test.cpp

example: example.cpp print.h
	c++ -O2 -Wall -std=gnu++0x -o example example.cpp

clean:
	rm test test