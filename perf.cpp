#include <iostream>
#include <sstream>
#include <time.h>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <stdio.h>
#include <vector>

#include "print.h"

static const int count = 3000000;
static const char string[] = "This is a string.";

void show_time(void (*f)(), char const *caption) { 
    clock_t start = clock();
    f();
    clock_t ticks = clock()-start;
    std::cerr << std::setw(40) << caption 
    << ": " 
    << (double)ticks/CLOCKS_PER_SEC << "\n";
}

void use_printf() {
    for (int i=0; i<count; i++)
        printf("string='%s' count='%d'\n", string, count);
}

// void use_printf_unlocked() {
//     for (int i=0; i<count; i++)
//         printf_unlocked("string='%s' count='%d'\n", string, count);
// }

void use_puts() {
    for (int i=0; i<count; i++) 
        puts(string);        
}

void use_cout() { 
    for (int i=0; i<count; i++)
        std::cout << "string='" << string << "' count='" << count << "'\n";
}

void use_cout_unsync() { 
    std::cout.sync_with_stdio(false);
    for (int i=0; i<count; i++)
        std::cout << "string='" << string << "' count='" << count << "'\n";
    std::cout.sync_with_stdio(true);
}

void use_stringstream() { 
    std::stringstream temp;
    for (int i=0; i<count; i++)
        temp << "string='" << string << "' count='" << count << "'\n";
    std::cout << temp.str();
}

void use_endl() { 
    for (int i=0; i<count; i++)
        std::cout << "string='" << string << "' count='" << count << "'" << std::endl;
}

void use_fill_n() { 
    std::fill_n(std::ostream_iterator<char const *>(std::cout, "\n"), count, string);
}

void use_prettyprint() {
    for (int i = 0; i < count; i++)
        print "string='%o' count='%o'" % string, count;
}

void use_putc() {
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < (int) sizeof(string); j++) {
            //fputc(string[j], stdout);
            putc(string[j], stdout);
        }
        
        putc('\n', stdout);
    }
}

// void use_fputs() {
//     fputs("string='", stdout);
//     fputs()
// }

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
        { &use_stringstream, "Time using stringstream"},
        { &use_endl, "Time using endl"},
        { &use_fill_n, "Time using fill_n"},
        { &use_prettyprint, "Time using pretty print statement"},
        { &use_putc, "Time using putc"}
    };
    
    srand(time(0));
    
    random_shuffle( cases.begin(), cases.end() );
    
    for ( auto const& thing : cases ) {
        show_time(thing.func, thing.desc);
    }
    
    
    return 0;
}
