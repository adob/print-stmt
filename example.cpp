#include <time.h>
#include <stdint.h>
#include "print.h"

int main() {
    print "Hello, world";
    print "one", "two", "three";
    print "time now:", time(NULL);
    print "Print: 1=%o 2=%o 3=%o" % 'a', "Alex", 65;
}