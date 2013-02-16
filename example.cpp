#include <time.h>
#include <stdint.h>


#include <string>
#include <vector>
#include <type_traits>
#include <unordered_set>

#include "print.h"

template <typename T, int>
struct TestInt {
    typedef T type;
};

template <typename T>
typename TestInt<void, sizeof ( (typename T::const_iterator (T::*) () const ) &T::begin ) >::type
Foo() {
    
}


struct S {
    void foo() {
        
    }
    
    int begin() const {
        return 3;
    }
};

int main() {
    
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
    
}