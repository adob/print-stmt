#pragma once
#include <stdio.h>

namespace awesome {

inline void Write(FILE *file, const char *str) {
    fputs(str, file);
}

inline void Write(FILE *file, int num) {
    fprintf(file, "%d", num);
}

inline void Write(FILE *file, long num) {
    fprintf(file, "%ld", num);
}

inline void Write(FILE *file, char c) {
    fprintf(file, "%c", c);
}

inline void Write(FILE *file, unsigned char c) {
    fprintf(file, "%d", c);
}

inline void Write(FILE *file, bool b) {
    fputs(b ? "true" : "false", file);
}

struct PrintFormatted
{
    FILE *file;
    const char *str;
    const char *percent;
    
    PrintFormatted(FILE *f, const char *s) : file(f), str(s)
    {
        do_print();
    }
    
    void do_print()
    {
        while (*str)
        {
            if (*str == '%' && *(str+1) != '%')
            {
                percent = str;
                str += 2;
                goto done;
            }
            
            fputc(*str, file);
            
            str++;
        }
        
        fputc('\n', file);
        percent = 0;
        
        done: ;
    }
    
    template <typename T>
    PrintFormatted& operator , (T t)
    {
        Write(file, t);
        do_print();
        return *this;
    }
    
    ~PrintFormatted() 
    {
        if (percent) {
            fputs(percent, file);
            fputc('\n', file);
        }
    }
 
};


struct PrintUnformatted
{
    FILE *file;
    
    PrintUnformatted(FILE *f) : file(f) {}
    
    template <typename T>
    PrintUnformatted& operator , (T t)
    {
        fputc(' ', file);
        Write(file, t);
        return *this;
    }
    
    ~PrintUnformatted() { 
        fputc('\n', file);
    }
    
};

struct PrintUndecided
{
    FILE *file;
    const char *str;
    
    PrintUndecided(FILE *f, const char *s) : file(f), str(s) {}
    
    template <typename T>
    PrintFormatted operator % (T t)
    {
        PrintFormatted pf(file, str);
        pf, t;
        str = 0;
        return pf;
    }
    
    template <typename T>
    PrintUnformatted operator , (T t)
    {
        fputs(str, file);
        fputc(' ', file);
        Write(file, t);
        str = 0;
        return PrintUnformatted(file);
    }
    
    ~PrintUndecided() { 
        if (str != 0) {
            Write(file, str);
            fputc('\n', file);
        }
        
    }
};

struct Print
{        
    FILE *file;
    
    Print()        : file(stdin) {}
    Print(FILE *f) : file(f)     {}
    
    template <typename T>
    PrintUnformatted operator * (T const& t) 
    {
        Write(file, t);
        return PrintUnformatted(file);
    }
    
    PrintUndecided operator * (const char *str) 
    {
        return PrintUndecided(file, str);
    }

};
}

#define print awesome::Print(stdout)*
#define warn awesome::Print(stderr)*

