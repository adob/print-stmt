all: test-gcc test-clang test-gcc-c11 test-clang-c11 example perf-gcc perf-clang

run-tests: test-gcc test-clang test-gcc-c11 test-clang-c11
	./test-gcc && ./test-gcc-c11 && echo "gcc all tests passed"
	./test-clang && ./test-clang-c11 && echo "clang all tests passed"

test-gcc test-clang test2-gcc test2-clang \
test-gcc-c11 test-clang-c11: test.cpp test2.cpp print.h
	g++ -g -O2 -Wall -fsanitize=address -std=c++98 -o test-gcc test.cpp
	clang++ -g -O2 -Wall -fsanitize=address -std=c++98 -o test-clang test.cpp
	g++ -g -O2 -Wall -fsanitize=address -std=c++1y -o test-gcc-c11 test.cpp
	clang++ -g -O2 -Wall -fsanitize=address -std=c++1y -o test-clang-c11 test.cpp
	g++ -g -O2 -Wall -std=c++98 -o test2-gcc test2.cpp
	clang++ -g -O2 -Wall -std=c++98 -o test2-clang test2.cpp
	g++ -g -O2 -Wall -std=c++1y -o test2-gcc-c11 test2.cpp
	clang++ -g -O2 -Wall -std=c++1y -o test2-clang-c11 test2.cpp

example: example.cpp print.h
	c++ -O2 -Wall -std=gnu++0x -o example example.cpp

	
perf-gcc perf-clang: perf.cpp print.h
	g++ -O2 -Wall -std=gnu++0x -o perf-gcc perf.cpp
	clang++ -O2 -Wall -std=gnu++0x -o perf-clang perf.cpp


clean:
	rm test-* example perf-*
