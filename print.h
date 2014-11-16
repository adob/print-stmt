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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// extern "C" {
//     size_t strlen(const char *);
//     void  *free(void *ptr);
// }

#ifdef __GXX_RTTI
#include <typeinfo>
extern "C" {
char*
__cxa_demangle(const char* __mangled_name, char* __output_buffer,
               size_t* __length, int* __status);

}
#endif

namespace std {
    template <typename T1, typename T2>
    struct pair;
    
    template <typename Key, typename T, typename Compare, typename Allocator>
    struct map;
    
    template<class Key, class T, class Hash, class KeyEqual, class Allocator> 
    struct unordered_map;
    
    template <typename CharT>
    struct char_traits;
    
    template < typename CharT, typename Traits>
    class basic_streambuf;
    
    template <typename charT, typename traits>
    class basic_ostream;
    
    typedef basic_ostream< char, char_traits<char> > ostream;
    typedef basic_streambuf< char, char_traits<char> > streambuf;
}

namespace pretty {

typedef unsigned Opt;
const Opt OptNone       = 1;
const Opt OptQuoted     = 1 << 1;
const Opt OptDone       = 1 << 2;
//const Opt OptStrPresent = 1 << 3;

    
template <typename T>
void WriteX(FILE *, T const&, bool);

inline void WriteCharQuoted(FILE *file, char c) {
    switch (c) {
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
        case '\0':
            fputs_unlocked("\\0", file); break;
        default:
            putc_unlocked(c, file);
    }
}

// inline void WriteChar(File *file, char c, bool quoted) {
//     if (quoted)
//         WriteCharQuoted(file, c, quoted);
//     else
//         putc_unlocked(c, file);
// }
    
inline void WriteX(FILE *file, const char *str, bool quoted) {
    if (!str) {
        fputs_unlocked("null", file);
    }
    else if (!quoted) {
        fputs_unlocked(str, file);
    }
    else {
        putc_unlocked('"', file);
        while (*str != '\0') {
            WriteCharQuoted(file, *str++);
        }
        putc_unlocked('"', file);
    }
}

inline void Write(FILE *file, const char *str, size_t len, bool quoted) {
    if (!str) {
        fputs_unlocked("null", file);
    }
    else if (!quoted)  {
        fwrite_unlocked(str, 1, len, file);
    }
    else {
        putc_unlocked('"', file);
        for (size_t i = 0; i < len; i++) {
            WriteCharQuoted(file, *str++);
        }
        putc_unlocked('"', file);
    }
}

inline void Write(FILE *file, char *str, bool quoted) {
    WriteX(file, (const char *) str, quoted);
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

inline void Write(FILE *file, short int num, bool) {
    Write(file, (int) num, false);
}
inline void Write(FILE *file, short unsigned int num, bool) {
    Write(file, (unsigned) num, false);
}

inline void Write(FILE *file, long int num, bool) {
    char buf[100];
    int cnt = snprintf(buf, sizeof buf, "%ld", num);
    
    fwrite_unlocked(buf, 1, cnt, file);
}

inline void Write(FILE *file, unsigned long int num, bool) {
    char buf[100];
    int cnt = snprintf(buf, sizeof buf, "%lu", num);
    
    fwrite_unlocked(buf, 1, cnt, file);
}

inline void Write(FILE *file, long long int num, bool) {
    char buf[100];
    int cnt = snprintf(buf, sizeof buf, "%lld", num);
    
    fwrite_unlocked(buf, 1, cnt, file);
}

inline void Write(FILE *file, unsigned long long int num, bool) {
    char buf[100];
    int cnt = snprintf(buf, sizeof buf, "%llu", num);
    
    fwrite_unlocked(buf, 1, cnt, file);
}

template <typename T>
inline void Write(FILE *file, T *p, bool) {
    char buf[256];
#ifdef __GXX_RTTI   
    int status;
    char *demangled = __cxa_demangle(typeid(p).name(), 0, 0, &status);
    int cnt = snprintf(buf, sizeof buf, "<%s at %p>", demangled, p);
    free(demangled);
#else
    int cnt = snprintf(buf, sizeof buf, "%p", p);
#endif
    fwrite_unlocked(buf, 1, cnt, file);
}

template <typename T, size_t N>
inline void WriteX(FILE *file, T (&arr)[N], bool quoted) {
    putc_unlocked('[', file);

    size_t i = 0;
    if (i < N) {
        WriteX(file, arr[i], quoted);
        
        while (++i < N) {
            fputs_unlocked(", ", file);
            WriteX(file, arr[i], quoted);
        }
    }
    
    putc_unlocked(']', file);
}
    

#ifdef __GXX_RTTI
inline void Write(FILE *file, std::type_info const& tinfo, bool) {
    int status;
    char *demangled = __cxa_demangle(tinfo.name(), 0, 0, &status);
    fwrite_unlocked(demangled, 1, strlen(demangled), file);
    free(demangled);
}
#endif

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
            case '\0':
                fputs_unlocked("\\0", file); break;
            default:
                putc_unlocked(c, file);
        }
        putc_unlocked('\'', file);
        
    }
}

inline void WriteUTF8(FILE *file, uint32_t c, bool quoted) {
    const uint32_t MAX_RUNE  = 0x0010FFFF; // Maximum valid Unicode code point.
    const uint32_t SURROGATE_MIN = 0xD800;
    const uint32_t SURROGATE_MAX = 0xDFFF;
    
    const uint32_t RUNE1_MAX = (1<<7)  - 1;
    const uint32_t RUNE2_MAX = (1<<11) - 1;
    const uint32_t RUNE3_MAX = (1<<16) - 1;
    const char TX = 0x80; // 1000 0000
    const char T2 = 0xC0; // 1100 0000
    const char T3 = 0xE0; // 1110 0000
    const char T4 = 0xF0; // 1111 0000
    const char MASK_X = 0x3F; // 0011 1111
    
    
    char buf[25];
    char *p = buf;
    if (quoted) *(p++) = '\'';
    if (c <= RUNE1_MAX) {
        Write(file, (char) c, quoted);
        return;
    } else if (c <= RUNE2_MAX) {
        *(p++) = T2 | (c >> 6);
        *(p++) = TX | (c & MASK_X);
    } else if (c > MAX_RUNE || (SURROGATE_MIN <= c && c <= SURROGATE_MAX)) {
        p += sprintf(p, "\\x%X", c);
    } else if (c <= RUNE3_MAX) {
        *(p++) = T3 | (c >> 12);
        *(p++) = TX | ((c >> 6) & MASK_X);
        *(p++) = TX | (c & MASK_X);
    } else {
        *(p++) = T4 | (c >> 18);
        *(p++) = TX | ((c >> 12) & MASK_X);
        *(p++) = TX | ((c >> 6) & MASK_X);
        *(p++) = TX | (c & MASK_X);
    }
    if (quoted) *(p++) = '\'';
    *(p++) = '\0';
    fputs_unlocked(buf, file);
}

#if defined(__GXX_EXPERIMENTAL_CXX0X__)
inline void Write(FILE *file, char32_t c, bool quoted) {
    WriteUTF8(file, (uint32_t)c, quoted);
}
inline void Write(FILE *file, char16_t c, bool quoted) {
    WriteUTF8(file, (uint32_t)c, quoted);
}
#endif
inline void Write(FILE *file, wchar_t c, bool quoted) {
    WriteUTF8(file, (uint32_t)c, quoted);
}

//TODO: fix this
inline void Write(FILE *file, __int128_t num, bool) {
    char buf[100];
    int cnt = snprintf(buf, sizeof buf, "%lld", (long long)num);
    
    fwrite_unlocked(buf, 1, cnt, file);
}
//TODO: fix this
inline void Write(FILE *file, __uint128_t num, bool) {
    char buf[100];
    int cnt = snprintf(buf, sizeof buf, "%llu", (unsigned long long)num);
    
    fwrite_unlocked(buf, 1, cnt, file);
}  

inline void Write(FILE *file, signed char c, bool) {
    Write(file, (int)c, false);
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

inline void Write(FILE *file, float f, bool) {
    Write(file, (double) f, true);
}
inline void Write(FILE *file, long double f, bool) {
    Write(file, (double) f, true);
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
    Write(file, s.c_str(), s.size(), quoted);
}


#ifdef QSTRING_H
inline void Write(FILE *file, QString const& str, bool quoted) 
{
    Write(file, str.toLocal8Bit().constData(), quoted);
}

#endif

template <typename T1, typename T2 >
void
Write(FILE *file, std::pair<T1, T2> const& pair, bool quoted)
{
    putc_unlocked('(', file);
    WriteX(file, pair.first, quoted);
    fputs_unlocked(", ", file);
    WriteX(file, pair.second, quoted);
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
        WriteX(file, *it, quoted);
        ++it;
        
        while (it != iterable.end()) {
            fputs_unlocked(", ", file);
                
            WriteX(file, *it, quoted);
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
        WriteX(file, it->first, quoted);
        fputs_unlocked(": ", file);
        WriteX(file, it->second, quoted);
        
        ++it;
        
        while (it != map.end()) {
            fputs_unlocked(", ", file);
            
            WriteX(file, it->first, quoted);
            fputs_unlocked(": ", file);
            WriteX(file, it->second, quoted);
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



#if defined(QHASH_H) || defined(QMAP_H)
template <typename T>
void WriteMap_Qt(FILE *file, T const& map, bool quoted) {
    const QList<typename T::key_type> keys = map.keys();
    
    fputs_unlocked("{ ", file);
    
    typename QList<typename T::key_type>::const_iterator it = keys.begin();
    
    if (it != keys.end()) {
        goto skip_comma;
    }
    else {
        goto done;
    }
    
    while (it != keys.end()) {
        
        fputs_unlocked(", ", file);
        
        skip_comma:
        WriteX(file, *it, quoted);
        fputs_unlocked(": ", file);
        WriteX(file, map.values(*it), quoted);
        ++it;
    }
    
    done:
    fputs_unlocked(" }", file);
}

#ifdef QHASH_H
template <typename Key, typename T>
void Write(FILE *file, QHash<Key, T> const& map, bool quoted) {
    WriteMap_Qt(file, map, quoted);
}
#endif


#ifdef QMAP_H
template <typename Key, typename T>
void Write(FILE *file, QMap<Key, T> const& map, bool quoted) {
    WriteMap_Qt(file, map, quoted);
}

template <typename Key, typename T>
void Write(FILE *file, QMultiMap<Key, T> const& map, bool quoted) {
    WriteMap_Qt(file, map, quoted);
}
#endif
#endif

template <typename T>
T to_instance();

template <typename T>
T& to_instance_ref();

template <typename T>
T* make_new(void *ptr) {
    struct M : public T {
        
    };
    
    M *mp = new (ptr) M();
    return mp;
}

template <typename T, typename STREAMBUF, typename OSTREAM>
void
WriteStream(FILE *file, T const& t, bool quoted) {
    struct OutputStream : public STREAMBUF {
        FILE *file;
        bool quoted;
        
        OutputStream(FILE *f, bool q) : file(f), quoted(q) {}
        
        ptrdiff_t xsputn(const char *s, ptrdiff_t cnt) {
            if (quoted) {
                for (ptrdiff_t i = 0; i < cnt; i++) {
                    WriteCharQuoted(file, *s++);
                }
            }
            else {
                fwrite_unlocked(s, cnt, 1, file);
            }
            
            return cnt;
        }
        
        typename STREAMBUF::int_type overflow(typename STREAMBUF::int_type ch) {
            if (quoted)
                WriteCharQuoted(file, ch);
            else
                putc_unlocked(ch, file);
            return 1;
        }
    };
    
    OutputStream buf(file, quoted);
    OSTREAM stream(&buf);
    if (quoted)
        putc_unlocked('"', file);
    stream << t;
    if (quoted)
        putc_unlocked('"', file);
}

template <typename T>
typename EnableIf2<void,
    sizeof( to_instance_ref< ::std::ostream >() << to_instance<T>() ),
    typename Test<  sizeof ( test_cstr<T>( 0 ) )  >::No
>::type
Write(FILE *file, T const& t, bool quoted) {
    WriteStream<T, ::std::streambuf, ::std::ostream>(file, t, quoted);
}


template <typename T>
void Write(FILE *file, T const& t, ...) {
#ifdef __GXX_RTTI   
    int status;
    char *demangled = __cxa_demangle(typeid(t).name(), 0, 0, &status);
    fprintf(file, "<%s>", demangled);
    free(demangled);
#else
    fputs_unlocked("<?>", file);
#endif
}

template <typename T>
void WriteX(FILE *file, T const& t, bool quoted) {
    Write(file, t, quoted);
}

struct PrintBase
{
    FILE       *file;
    const char *str;
    unsigned    opts;
    bool        str_present;
} ;

struct PrintFormatted : PrintBase
{
    void init_PrintFormatted() {
        do_print();
    }
    
    void do_print() {
        for (; *str; str++)
        {
            if (*str == '%')
            {
                switch (str[1]) {
                    case '\0':
                    case '%':
                        goto skip;
                        
                    case 'q':
                        opts = OptQuoted;
                        break;
                    default:
                        opts = OptNone;
                        break;
                }
                
                str += 2;
                return;
            }
            
            skip:
            putc_unlocked(*str, file);
            
        }
        
        opts = OptNone | OptDone;
    }
    
    template <typename T>
    PrintFormatted& operator , (T const& t) {
        bool quoted = opts & OptQuoted;
        
        if (opts & OptDone)
            putc_unlocked(' ', file);
        
        WriteX(file, t, quoted);
        
        if (~opts & OptDone)
            do_print();
        
        return *this;
    }

};


struct PrintUnformatted : PrintFormatted
{
    template <typename T>
    PrintUnformatted& operator , (T const& t) {
        putc_unlocked(' ', file);
        WriteX(file, t, false);
        return *this;
    }
};

struct PrintUndecided : PrintUnformatted
{
    
    void init_PrintUndecided(const char *s) {
        this->str = s;
        this->str_present = true;
    }
    
    template <typename T>
    PrintFormatted& operator % (T const& t)
    {
        PrintFormatted& pf = *this;
        init_PrintFormatted();
        pf, t;
        this->str_present = false;
        return *this;
    }
    
    template <typename T>
    PrintUnformatted& operator , (T const& t)
    {
        WriteX(file, str, false);
        putc_unlocked(' ', file);
        WriteX(file, t, false);
        this->str_present = false;
        return *this;
    }
    
};

struct Print : PrintUndecided
{
    
    Print(FILE *f = stdout) { 
        this->file = f;
        this->opts = 0;
        this->str_present = false;
        flockfile(f);
    }
    
    template <typename T>
    PrintUnformatted& operator * (T const& t) {

        WriteX(file, t, false);
        return *this;
    }
    
    PrintUndecided& operator * (const char *str) {
        init_PrintUndecided(str);
        return *this;
    }
    
    ~Print() {
        if ((opts & OptNone) && !(opts & OptDone)) {
            fputs_unlocked(str-2, file);
        } else if (str_present) {
            WriteX(file, str, false);
        }
    
        putc_unlocked('\n', file);
        funlockfile(file);
    }

};
}

#ifndef PRETTYPRINT_NO_KEYWORD
#define print  (::pretty::Print(stdout))*
#define eprint (::pretty::Print(stderr))*
#endif

