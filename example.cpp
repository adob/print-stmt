#include <time.h>
#include <stdint.h>
#include "print.h"

#include <string>
#include <vector>
#include <type_traits>


int main() {
    
    std::string str = "Hello World";
    std::vector<int> vec = {23, 44, 645};
    
    print "str:", str;
    print "vec:", vec;
}