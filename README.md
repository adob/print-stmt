### Usage ###

Just inclde `print.h` header at the top of the file.
    
    #include "print.h"
    
### Examples ###

    print "Hello, world";
    >> Hello, world
    
    print "one", "two", "three";
    >> one two three

    print "time now:", time(NULL);
    >> time now: 1360826578
    
    print "Print: 1=%o 2=%o 3=%o" % 'a', "Alex", 65;
    >> Print: 1=a 2=Alex 3=65