all: test-gcc test-clang example perf-gcc perf-clang

run-tests: test-gcc test-clang
	./test-gcc && echo "gcc all tests passed"
	./test-clang && echo "clang all tests passed"

test-gcc test-clang test2-gcc test2-clang: test.cpp test2.cpp print.h
	g++-4.8 -g -O2 -Wall -fsanitize=address -std=c++98 -o test-gcc test.cpp
	clang++ -g -O2 -Wall -fsanitize=address -std=c++98 -o test-clang test.cpp
	g++-4.8 -g -O2 -Wall -std=c++98 -o test2-gcc test2.cpp
	clang++ -g -O2 -Wall -std=c++98 -o test2-clang test2.cpp

example: example.cpp print.h
	c++ -O2 -Wall -std=gnu++0x -o example example.cpp

	
perf-gcc perf-clang: perf.cpp print.h
	g++-4.8 -O2 -Wall -std=gnu++0x -o perf-gcc perf.cpp
	clang++ -O2 -Wall -std=gnu++0x -o perf-clang perf.cpp


clean:
	rm test-* example perf-*
