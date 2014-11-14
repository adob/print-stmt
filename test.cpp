#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


#define PRETTYPRINT_NO_KEYWORD 1
#include "print.h"



using namespace pretty;

char buf[1024 * 64];
FILE *file;
int testcnt = 0;

void do_test(int testno, const char *expected, int lineno);
void clear_file();

struct MyClass {
    
};

namespace foo {
    class Bar {
        
    };
}

#define TEST(expected) do_test(++testcnt, expected, __LINE__)

void more_tests() {
    {
        clear_file();
        long long i = 42;
        pretty::Print(file) * i;
        TEST("42\n");
    }
    
    {
        clear_file();
        long long int i = 42;
        pretty::Print(file) * i;
        TEST("42\n");
    }
}

#include <string>
#include <vector>

int main() {
    clear_file();
    fprintf(file, "hello\n");
    TEST("hello\n");
    
    clear_file();
    pretty::Print(file) * "hello";
    TEST("hello\n");
    
    clear_file();
    pretty::Print(file) * "hello", "one";
    TEST("hello one\n");
    
    clear_file();
    pretty::Print(file) * "hello", "one", "two";
    TEST("hello one two\n");
    
    clear_file();
    pretty::Print(file) * "";
    TEST("\n");
    
    clear_file();
    pretty::Print(file) * "hello %o" % 3;
    TEST("hello 3\n");
    
    {
        clear_file();
        char tmp[100];
        foo::Bar bar;
        sprintf(tmp, "p = <foo::Bar* at %p>\n", &bar);
        pretty::Print(file) * "p =", &bar; 
        TEST(tmp);
    }
    
    {
        clear_file();
        char c = 'c';
        pretty::Print(file) * c; 
        TEST("c\n");
    }
    
    {
        clear_file();
        uint8_t v = 224;
        pretty::Print(file) * v; 
        TEST("224\n");
    }
    
    {
        clear_file();
        int8_t v = -50;
        pretty::Print(file) * v; 
        TEST("-50\n");
    }
    
    clear_file();
    pretty::Print(file) * 2.27; 
    TEST("2.27\n");
    
    clear_file();
    pretty::Print(file) * 2.27f; 
    TEST("2.27\n");

    
    {
        clear_file();
        std::string str = "This is a std::string.";
        
        pretty::Print(file) * "hello from <%o>" % str;
        
        
        TEST("hello from <This is a std::string.>\n");
    }
    
    {
        clear_file();
        std::vector<int> vec;
        vec.push_back(1);
        vec.push_back(2);
        vec.push_back(3);
        std::pair< std::vector<int>, int> p(vec, 1);
        
        pretty::Print(file) * p;
        TEST("([1, 2, 3], 1)\n");
    }
    
    {
        clear_file();
        pretty::Print(file) * "abc%" % 1;
        TEST("abc% 1\n");
    }
    
    {
        clear_file();
        pretty::Print(file) * "abc %s %s %" % "A";
        TEST("abc A %s %\n");
    }
    
    {
        clear_file();
        pretty::Print(file) * "abc %s %s %" % "A", "B", "C", "D";
        TEST("abc A B % C D\n");
    }
    
    {
        //clear_file();
        //pretty::Print(file) * MyClass();
        //TEST("<MyClass>\n");
        
    }
    
    {
        clear_file();
        const char *str = 0;
        pretty::Print(file) * str;
        TEST("null\n");
    }
    
    {
        clear_file();
        const char *str = 0;
        pretty::Print(file) * "foo", str;
        TEST("foo null\n");
    }
    
    #if defined(__GXX_EXPERIMENTAL_CXX0X__)
    {
        clear_file();
        char32_t c = 233;
        pretty::Print(file) * c;
        TEST("é\n");
    }
    {
        clear_file();
        char16_t c = 233;
        pretty::Print(file) * c;
        TEST("é\n");
    }
    #endif
    
    {
        clear_file();
        wchar_t c = 233;
        pretty::Print(file) * c;
        TEST("é\n");
    }
    
    {
        clear_file();
        bool b = true;
        pretty::Print(file) * b;
        TEST("true\n");
    }
    
    {
        clear_file();
        int numbers[] = { 1, 2, 3 };
        pretty::Print(file) * numbers;
        TEST("[1, 2, 3]\n");
    }
    

    more_tests();
}


void clear_file() {
    if (file) {
        fclose(file);
    }
    
    file = fmemopen(buf, sizeof buf, "w");
    
    if (!file) {
        perror("fmemopen failed");
        exit(1);
    }
}


void do_test(int testno, const char *expected, int lineno) {
    int expected_len  = (int) strlen(expected);
    int file_len      = (int) ftell(file);
    
    fflush(file);
    
    if (strcmp(buf, expected) != 0) {
        fprintf(stderr, "%s:%d: Test %d failed: Not equal: \n", __FILE__, lineno, testno);
        fprintf(stderr, "  Got      = <%s>\n", buf);
        fprintf(stderr, "  Expected = <%s>\n", expected);
        exit(2);
    }
    
    if ( expected_len != file_len ) {
        fprintf(stderr, "%s:%d: Test %d failed: Size mismatch: \n", __FILE__, lineno, testno);
        fprintf(stderr, "  Got      = %d\n", file_len);
        fprintf(stderr, "  Expected = %d\n", expected_len);
        
        exit(2);
        
    }
    
}

#include <ostream>
std::ostream& operator << (std::ostream& s, MyClass const& o) {
    s << "<MyClass>";
    return s;
}