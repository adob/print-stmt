all: test example perf

run-tests: test
	./test && echo "all tests passed"

test: test.cpp print.h
	c++ -O2 -Wall -std=c++98 -Wall -o test test.cpp

example: example.cpp print.h
	c++ -O2 -Wall -std=gnu++0x -o example example.cpp

	
perf: perf.cpp print.h
	c++ -O2 -Wall -std=gnu++0x -o $@ $@.cpp

clean:
	rm test example perf