// Copyright (c) 2013 Aleksandr Dobkin
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once
#include <stdio.h>

namespace std {
    template <typename T1, typename T2>
    struct pair;
    
    template <typename Key, typename T, typename Compare, typename Allocator>
    struct map;
    
    template<class Key, class T, class Hash, class KeyEqual, class Allocator> 
    struct unordered_map;
}

namespace pretty {

inline void Write(FILE *file, const char *str, bool quoted) {
    if (!quoted) 
    {
        fputs_unlocked(str, file);
    }
    else 
    {
        putc_unlocked('"', file);
        while (*str != '\0') {
            switch (*str) {
                case '\r':
                    fputs_unlocked("\\r", file); break;
                case '\n':
                    fputs_unlocked("\\n", file); break;
                case '\t':
                    fputs_unlocked("\\t", file); break;
                case '\\':
                    fputs_unlocked("\\\\", file); break;
                case '"':
                    fputs_unlocked("\\\"", file); break;
                default:
                    putc_unlocked(*str, file);
            }
            
            str++;
        }
        putc_unlocked('"', file);
    }
}

inline void Write(FILE *file, int num, bool) {
    char buf[100];
    int cnt = snprintf(buf, sizeof buf, "%d", num);
    
    fwrite_unlocked(buf, 1, cnt, file);
}

inline void Write(FILE *file, unsigned int num, bool) {
    char buf[100];
    int cnt = snprintf(buf, sizeof buf, "%u", num);
    
    fwrite_unlocked(buf, 1, cnt, file);
}

inline void Write(FILE *file, long num, bool) {
    char buf[100];
    int cnt = snprintf(buf, sizeof buf, "%ld", num);
    
    fwrite_unlocked(buf, 1, cnt, file);
}

inline void Write(FILE *file, unsigned long num, bool) {
    char buf[100];
    int cnt = snprintf(buf, sizeof buf, "%lu", num);
    
    fwrite_unlocked(buf, 1, cnt, file);
}

inline void Write(FILE *file, void *p, bool) {
    char buf[100];
    int cnt = snprintf(buf, sizeof buf, "%p", p);
    
    fwrite_unlocked(buf, 1, cnt, file);
}

inline void Write(FILE *file, char c, bool quoted) {
    if (!quoted)
        putc_unlocked(c, file);
    else {
        putc_unlocked('\'', file);
        
        switch (c) {
            case '\r':
                fputs_unlocked("\\r", file); break;
            case '\n':
                fputs_unlocked("\\n", file); break;
            case '\t':
                fputs_unlocked("\\t", file); break;
            case '\\':
                fputs_unlocked("\\\\", file); break;
            case '\'':
                fputs_unlocked("\\\'", file); break;
            default:
                putc_unlocked(c, file);
        }
        putc_unlocked('\'', file);
        
    }
}

inline void Write(FILE *file, unsigned char c, bool) {
    char buf[100];
    int cnt = snprintf(buf, sizeof buf, "%d", c);
    
    fwrite_unlocked(buf, 1, cnt, file);
}

inline void Write(FILE *file, double c, bool) {
    char buf[100];
    int cnt = snprintf(buf, sizeof buf, "%g", c);
    
    fwrite_unlocked(buf, 1, cnt, file);
}

inline void Write(FILE *file, bool b, bool) {
    fputs_unlocked(b ? "true" : "false", file);
}

template < typename ReturnType, int > 
struct EnableIfInt { 
    typedef ReturnType type; 
};

template <typename STRING>
typename EnableIfInt<void, 
    sizeof (
        (const char* (STRING::*) () const) &STRING::c_str 
    )
>::type
Write(FILE *file, STRING const& s, bool quoted) {
    Write(file, s.c_str(), quoted);
}


#ifdef QSTRING_H
void Write(FILE *file, QString const& str, bool quoted) 
{
    Write(file, str.toLocal8Bit().constData(), quoted);
}

#endif

#ifdef QHASH_H
template <typename Key, typename T>
void Write(FILE *file, QHash<Key, T> const& map, bool quoted)
{
    const QList<Key> keys = map.keys();
    
    fputs_unlocked("{ ", file);
    
    typename QList<Key>::const_iterator it = keys.begin();
    
    if (it != keys.end()) {
        Write(file, *it, quoted);
        fputs_unlocked(": ", file);
        Write(file, map[*it], quoted);
        
        ++it;
        
        while (it != keys.end()) {
            fputs_unlocked(", ", file);
            
            Write(file, *it, quoted);
            fputs_unlocked(": ", file);
            Write(file, map[*it], quoted);
            ++it;
        }
    }
    
    fputs_unlocked(" }", file);
}
#endif

#ifdef QMAP_H
template <typename Key, typename T>
void Write(FILE *file, QMap<Key, T> const& map, bool quoted)
{
    const QList<Key> keys = map.keys();
    
    fputs_unlocked("{ ", file);
    
    typename QList<Key>::const_iterator it = keys.begin();
    
    if (it != keys.end()) {
        Write(file, *it, quoted);
        fputs_unlocked(": ", file);
        Write(file, map[*it], quoted);
        
        ++it;
        
        while (it != keys.end()) {
            fputs_unlocked(", ", file);
            
            Write(file, *it, quoted);
            fputs_unlocked(": ", file);
            Write(file, map[*it], quoted);
            ++it;
        }
    }
    
    fputs_unlocked(" }", file);
}
#endif

template < typename ReturnType, typename T > 
struct EnableIfType { 
    typedef ReturnType type; 
};


template <typename T1, typename T2 >
void
Write(FILE *file, std::pair<T1, T2> const& pair, bool quoted)
{
    putc_unlocked('(', file);
    Write(file, pair.first, quoted);
    fputs_unlocked(", ", file);
    Write(file, pair.second, quoted);
    putc_unlocked(')', file);
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


template < typename ReturnType, int,
          typename T2> 
struct EnableIf2 { 
    typedef ReturnType type; 
};


template <typename Iterable>
typename EnableIf2< void, 
    sizeof( 
        (typename Iterable::const_iterator (Iterable::*) () const)
            &Iterable::begin 
    ),
    
    typename Test<  sizeof ( test_cstr<Iterable>( 0 ) )  >::No
>::type
Write(FILE *file, Iterable const& iterable, bool quoted) 
{
    typename Iterable::const_iterator it = iterable.begin();
    
    putc_unlocked('[', file);
    
    if (it != iterable.end()) {
        Write(file, *it, quoted);
        ++it;
        
        while (it != iterable.end()) {
            fputs_unlocked(", ", file);
                
            Write(file, *it, quoted);
            ++it;
        }
    }
    
    putc_unlocked(']', file);
}


template <typename T>
void
WriteMap_Pairs(FILE *file, T const& map, bool quoted)
{
    typename T::const_iterator it = map.begin();
    
    fputs_unlocked("{ ", file);
    
    if (it != map.end()) {
        Write(file, it->first, quoted);
        fputs_unlocked(": ", file);
        Write(file, it->second, quoted);
        
        ++it;
        
        while (it != map.end()) {
            fputs_unlocked(", ", file);
            
            Write(file, it->first, quoted);
            fputs_unlocked(": ", file);
            Write(file, it->second, quoted);
            ++it;
        }
    }
    
    fputs_unlocked(" }", file);
}

template <typename Key, typename T, typename Compare, typename Allocator>
void
Write(FILE *file, ::std::map<Key, T, Compare, Allocator> const& map, bool quoted)
{
    WriteMap_Pairs(file, map, quoted);
}

template <class Key, class T, class Hash, class KeyEqual, class Allocator>
void
Write(FILE *file, ::std::unordered_map<Key, T, Hash, KeyEqual, Allocator> const& map,
      bool quoted)
{
    WriteMap_Pairs(file, map, quoted);
}

// template <typename T>
// T&
// DeclVal();
// 
// 
// // template <typename T>
// // typename EnableIfInt<void, 
// //     sizeof (
// //         &( DeclVal<QDataStream>() << DeclVal<T>() )
// //    )>::type
// // Write(FILE *, T & )
// // {
// //     //
// // }


struct PrintFormatted
{
    FILE *file;
    const char *str;
    const char *percent;
    
    PrintFormatted(FILE *f, const char *s) : file(f), str(s)
    {
        //puts("created PrintFormatted");
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
            
            putc_unlocked(*str, file);
            
            str++;
        }
        
        putc_unlocked('\n', file);
        percent = 0;
        
        done: ;
    }
    
        template <typename T>
    PrintFormatted& operator , (T const& t)
    {
        bool quoted = (percent[1] == 'q');
        Write(file, t, quoted);
        do_print();
        return *this;
    }
    
    ~PrintFormatted() 
    {
        //puts("destoryed PrintFormatted");
        if (percent) {
            fputs_unlocked(percent, file);
            putc_unlocked('\n', file);
        }
    }
 
};


struct PrintUnformatted
{
    FILE *file;
    
    PrintUnformatted(FILE *f) : file(f) { 
        //puts("created PrintUnformatted"); 
    }
    
    template <typename T>
    PrintUnformatted& operator , (T t)
    {
        putc_unlocked(' ', file);
        Write(file, t, false);
        return *this;
    }
    
    ~PrintUnformatted() {
        //puts("destoryed PrintUnformatted");
        putc_unlocked('\n', file);
    }
    
};

struct PrintUndecided
{
    FILE *file;
    const char *str;
    
    PrintUndecided(FILE *f, const char *s) : file(f), str(s) 
    { 
        //puts("created PrintUndecided"); 
    }
    
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
        fputs_unlocked(str, file);
        putc_unlocked(' ', file);
        Write(file, t, false);
        str = 0;
        return PrintUnformatted(file);
    }
    
    ~PrintUndecided() {
        //puts("destoryed PrintUndecided");
        if (str != 0) {
            fputs_unlocked(str, file);
            putc_unlocked('\n', file);
        }
        
    }
};

struct Print
{        
    FILE *file;
    
    Print(FILE *f = stdout) : file(f)
    { 
        //puts("created Print"); 
        flockfile(f);
    }
    
    template <typename T>
    PrintUnformatted operator * (T const& t) 
    {
        Write(file, t, false);
        return PrintUnformatted(file);
    }
    
    PrintUndecided operator * (const char *str) 
    {
        return PrintUndecided(file, str);
    }
    
    ~Print() {
        //puts("destoryed Print");
        funlockfile(stdout);
    }

};
}

#define print (::pretty::Print(stdout))*
#define warn  (::pretty::Print(stderr))*

