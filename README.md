An implementation of a `print` statement in C++.

```C++
# include "print.h"

print expression[, expression]*
print "format" % expression[, expression]*
eprint expression[, expression]*
eprint "format" % expression[, expression]*

/** examples **/
print "Hello", "world", time(0);       // Hello world 1360826578
print "name=%q age=%s" % "Alex", 26;   // name="Alex" age=26

int numbers[] = { 1, 2, 3 };
print numbers;                         // [1, 2, 3]

std::vector<std::string> things = {"apples", "oranges", "elephants"};
print things;                          // [apples, oranges, elephants]

```
The first form converts each expression into a string and prints it to standard output, with each expression separated by a space and a terminating newline at the end.
**eprint** is similar except it prints to standard error.

The second form uses a format string similar to printf().

- <code>**%s**</code> The argument is printed directly.
- <code>**%q**</code> The argument is quoted.

Expressions are converted to strings according to their type, as follows:

- `char*` are output as NUL-terminated strings like %s.
- `int`, `short`, `long`, `signed char`, `unsigned char` are printed in decimal notation like with %d.
- `char` prints the charcter like %c.
- `void *` or any other kind of pointers prints ths pointer in hex like %p.
- `double`, `float` prints the number in decimal notation like %g.
- `bool` prints 'true' or 'false'
- A type that defines a member function called `c_str()` is converted to a string by calling this function.
- A type than can be streamed to an `std::ostream` like `std::cout << thing` but does not have a `c_str()` member function is converted to a string using `<<`.
- A type that has a `begin()` member function that returns in iterator but does not have a `c_str()` member function is printed like a list, e.g. `["one", "two"]`.
- `std:map`, `std::unordered_map`, `std::multimap`, `std::unordered_multipmap`, `QHash`, `QMap`, `QMultiHash`, `QMultiMap` are special-cased and printed like dictionaries, e.g. `{"foo": "bar"}`.
- `QString` is printed as a string.
- `std::pair` is rendered as `("first", "second")`.


#### Usage ####
The easiest way to use this is to clone this repo and copy the file `print.h` into your project.
Then simply
```C++
# include "print.h"` 
```
at the top of the file, but after the system `#includes`s.

#### Misc####
`print` is thread and exception safe.
The output stream is locked once before anything is printed and then unlocked at the end inside a destructor. This ensures that the whole line will be printed without being interleaved with output from another thread.

Internally, the `fwrite()` family of function from C standard library are used for output. C++ streams are not used at all.

In benchmakrs `print` is a little bit faster than `cout` and a little bit slower than `printf`.

`print.h` includes `stdio.h` and nothing else.

