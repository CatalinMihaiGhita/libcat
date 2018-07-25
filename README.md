## Welcome to libCat

This is a C++17 library which implements concepts from the Category Theory.
The C++ standard library partialy does this, but we put it in a different view.
The main objective is to enforce the programmer to write correct code,
with lesser undefined behaviour.

### Algebraic Types

```C++

all<A,B,...> = (A, B, ...)
all<> = nil = ()
any<A,B,...> = A | B | ...
any<> = nevr = Void

```

