#include "print.h"

int main() {
    const char *str = "hello";
    print str;
    
    {
        long long i = 42;
        print i;
    }
    {
        unsigned long long i = 42;
        print i;
    }
    #if defined(__GXX_EXPERIMENTAL_CXX0X__)
    {
        char32_t a = 'x';
        char16_t b = 'y';
        print a, b;
    }
    #endif
    print (__int128) 1;
    print (unsigned __int128) 1;
    print (wchar_t) 'x', (wchar_t) 233;
    print (long double) 2.3;
}