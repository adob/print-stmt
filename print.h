#pragma once
#include <stdio.h>
#include <string>
//#include <type_traits>

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

template < typename ReturnType, typename ExpectedType, ExpectedType > 
struct EnableIf { 
    typedef ReturnType type; 
};

template <typename STRING>
typename EnableIf<void, const char* (STRING::*) () const, &STRING::c_str >::type
Write(FILE *file, STRING const& s) {
    fputs(s.c_str(), file);
}

struct Yes { char a; };
struct No  { char a; char b; };

template <typename T, const char * (T::*) () const>
struct TryCStr {
    typedef T* type;
    
};

template <typename T>
Yes
test_cstr( typename TryCStr<T, &T::c_str>::type t = 0);

template <typename T>
No
test_cstr( ... );


template < int >
struct Test {
    
};

template <>
struct Test<1> {
    typedef int Yes;
};

template <>
struct Test<2> {
    typedef int No;
};


template < typename ReturnType, typename ExpectedType1, ExpectedType1,
          /*typename ExpectedType2, ExpectedType2 */
          typename T2> 
struct EnableIf2 { 
    typedef ReturnType type; 
};


template <typename Iterable>
typename EnableIf2< void, typename Iterable::const_iterator (Iterable::*) () const, &Iterable::begin,
                          typename Test<  sizeof ( test_cstr<Iterable>( 0 ) )  >::No
                  >::type
Write(FILE *file, Iterable const& iterable) 
{
    typename Iterable::const_iterator it = iterable.begin();
    
    fputc('[', file);
    
    if (it != iterable.end()) {
        Write(file, *it);
        ++it;
        
        while (it != iterable.end()) {
            fputc(',', file);
            fputc(' ', file);
                
            Write(file, *it);
            ++it;
        }
    }
    
    fputc(']', file);
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
    PrintFormatted& operator , (T const& t)
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
    PrintUnformatted operator , (T const& t)
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

#define print (::awesome::Print(stdout))*
#define warn  (::awesome::Print(stderr))*

