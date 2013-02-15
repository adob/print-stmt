#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "print.h"


using namespace awesome;

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
    awesome::Print(file) * "hello";
    TEST("hello\n");
    
    clear_file();
    awesome::Print(file) * "hello", "one";
    TEST("hello one\n");
    
    clear_file();
    awesome::Print(file) * "hello", "one", "two";
    TEST("hello one two\n");
    
    clear_file();
    awesome::Print(file) * "";
    TEST("\n");
    
    clear_file();
    awesome::Print(file) * "hello %o" % 3;
    TEST("hello 3\n");
    
    
    {
        clear_file();
        std::string str = "This is a std::string.";
        
        awesome::Print(file) * "hello from <%o>" % str;
        
        
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