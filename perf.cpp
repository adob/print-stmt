#include <iostream>
#include <sstream>
#include <time.h>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <stdio.h>
#include <vector>
#include <string.h>

#include "print.h"

int count = 0;
static const int upto = 3000000;
static const char string[] = "This is a string.";

void use_cout_unsync();

void show_time(void (*f)(), char const *caption) { 
    if (f == use_cout_unsync)
        std::cout.sync_with_stdio(false);
    
    clock_t start = clock();
    
    
    for (count = 0; count < upto; count++)
        f();
    
    
    clock_t ticks = clock()-start;
    std::cerr << std::setw(40) << caption 
    << ": " 
    << (double)ticks/CLOCKS_PER_SEC << "\n";
    
    if (f == use_cout_unsync)
        std::cout.sync_with_stdio(true);
}

void use_printf() {
    printf("string='%s' count='%d'\n", string, count);
}

// void use_printf_unlocked() {
//     for (int i=0; i<count; i++)
//         printf_unlocked("string='%s' count='%d'\n", string, count);
// }

void use_puts() {
    puts(string);        
}

void use_cout() { 
    std::cout << "string='" << string << "' count='" << count << "'\n";
}

void use_cout_unsync() { 
    std::cout << "string='" << string << "' count='" << count << "'\n";
}

void use_stringstream() { 
    std::stringstream temp;
    
    temp << "string='" << string << "' count='" << count << "'\n";
    
    std::cout << temp.str();
}

void use_endl() { 
    
    std::cout << "string='" << string << "' count='" << count << "'" << std::endl;
}

void use_fill_n() { 
    std::fill_n(std::ostream_iterator<char const *>(std::cout, "\n"), count, string);
}

void use_prettyprint() {
    print "string='%o' count='%o'" % string, count;
}

void use_putc() {
    
    for (int j = 0; j < (int) sizeof(string); j++) {
        //fputc(string[j], stdout);
        putc(string[j], stdout);
    }
    
    putc('\n', stdout);

}

void use_fputs_unlocked() {
    flockfile(stdout);
    
    const char *s1 = "string='";
    const char *s2 = "' count='";
    const char *s3 = "'\n";
    char buf[100];
    
    fputs_unlocked(s1, stdout);
    fputs_unlocked(string, stdout);
    fputs_unlocked(s2, stdout);
    
    snprintf(buf, sizeof buf, "%d", count);
    fputs_unlocked(buf, stdout);
    
    fputs_unlocked(s3, stdout);
    
    funlockfile(stdout);
}


void use_fwrite_unlocked() {
    flockfile(stdout);
    
    const char s1[] = "string='";
    const char s2[] = "' count='";
    const char s3[] = "'\n";
    char buf[100];
    
    fwrite_unlocked(s1, sizeof s1, 1, stdout);
    fwrite_unlocked(string, strlen(string), 1, stdout);
    fwrite_unlocked(s2, sizeof s2, 1, stdout);
    
    int cnt = snprintf(buf, sizeof buf, "%d", count);
    fwrite_unlocked(buf, cnt, 1, stdout);
    
    fwrite_unlocked(s3, sizeof s3, 1, stdout);
    
    funlockfile(stdout);
}


void use_memcpy() {
    
    
    const char s1[] = "string='";
    const char s2[] = "' count='";
    const char s3[] = "'\n";
    char buf[256];
    char *ptr = buf;
    
    memcpy(ptr, s1, sizeof(s1)-1);
    ptr += sizeof(s1)-1;
    
    int size = strlen(string);
    memcpy(ptr, string, size);
    ptr += size;
    
    memcpy(ptr, s2, sizeof(s2)-1);
    ptr += sizeof(s2)-1;

    ptr += snprintf(ptr, 100, "%d", count);
    
    memcpy(ptr, s3, sizeof(s3)-1);
    ptr += sizeof(s3)-1;
    
    fwrite(buf, ptr-buf, 1, stdout);
    
}

void use_fputs() {
    
    const char *s1 = "string='";
    const char *s2 = "' count='";
    const char *s3 = ";\n";
    char buf[100];
    
    fputs(s1, stdout);
    fputs(string, stdout);
    fputs(s2, stdout);
    
    snprintf(buf, sizeof buf, "%d", count);
    fputs(buf, stdout);
    
    fputs(s3, stdout);
}

int main() { 
    
    struct PerfCase {
        void (*func) ();
        const char *desc;
    };
    
    std::vector<PerfCase> cases = { 
        { &use_printf, "Time using printf" },
        { &use_puts, "Time using puts" },
        { &use_cout, "Time using cout (synced)"},
        { &use_cout_unsync, "Time using cout (un-synced)"},
        //{ &use_stringstream, "Time using stringstream"},
        { &use_endl, "Time using endl"},
        //{ &use_fill_n, "Time using fill_n"},
        { &use_prettyprint, "Time using pretty print statement"},
        { &use_putc, "Time using putc"},
        { &use_fputs_unlocked, "Time using fputs_unlocked"},
        { &use_fputs, "Time using fputs"},
        { &use_fwrite_unlocked, "Time using fwrite_unlocked"},
        { &use_memcpy, "Time using memcpy"}
    };
    
    srand(time(0));
    
    //random_shuffle( cases.begin(), cases.end() );
    
//      show_time(use_memcpy, "x");
//      return 0;
    for ( auto const& thing : cases ) {
        show_time(thing.func, thing.desc);
    }
    
    
    return 0;
}
