#include <time.h>
#include <stdint.h>


#include <string>
#include <vector>
#include <type_traits>
#include <unordered_set>
//#include <iostream>

#include "print.h"

template <typename T, int>
struct TestInt {
    typedef T type;
};

template <typename T>
typename TestInt<void, 3>::type
Foo() {
    
}


struct S {
    void foo() {
        
    }
    
    int begin() const {
        return 3;
    }
};

char buf3[4] = "123";

int main() {
    print "Hello";
    
    const char *buf1 = "345";
    char buf2[] = "123";
    print "str %s %s %s" % buf1, buf2, buf3;
    print "str %s" % buf3;
    
    std::string str = "Hello World";
    std::vector<int> vec = {23, 44, 645};
    
    
    print "str:", str;
    print "vec:", vec;
    
    const std::string conststr = "Hello";
    print "conststr:", conststr;
    
    std::vector<std::string> strings = { "one", "two", "three" };

    print "strings:", strings;
     
    std::unordered_set<std::string> files = { "file1", "file2", "file3" };
    
    print "files:", files;
    
    print "abc %o %o %o def" % 1, 2, 3;
    
    long long int x = 3;
    pretty::Print(stdout) * x;
    
}