#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <stdint.h>

#include "print.h"



using namespace pretty;

char buf[1024 * 64];
FILE *file;
int testcnt = 0;

void do_test(int testno, const char *expected, int lineno);
void clear_file();


#define TEST(expected) do_test(++testcnt, expected, __LINE__)

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
        sprintf(tmp, "p = %p\n", file);
        pretty::Print(file) * "p =", file; 
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