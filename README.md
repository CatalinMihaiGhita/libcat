## Welcome to libCat

This is a C++17 library which implements concepts from the Category Theory.
The C++ standard library partialy does this, but we put it in a different view.
The main objective is to enforce the programmer to write correct code,
with lesser undefined behaviour.

### Algebraic Types

#### Product types
The tuple type: (A, B, ...)
```C++
tup<A, B, ...>
```
The unit type ()
```C++
unit = tup<>
```

The unit type singletone
```C++
nil
```

#### Sum types
The variant type: A | B | ...
```C++
var<A, B, ...>
```
The zero type
```C++
nvr = var<>
```

### Memory Management Types

A unique_ptr<A> equivalent, but never nullptr
```C++
box<A>
```
  
A shared_ptr<A> equivalent, but never nullptr
```C++
rc<A> 
```

Memory management of A with a linked list
```C++
link<A>
```

### Monad Types

The optional type: A | unit
```C++
opt<A> = var<A, unit> 
```

The lazy type: A | nvr
```C++
lazy<A> = var<A, nvr>
```

The vector type (A, ...) | unit
```C++
vec<A>
```

### Monad Types + Memory Management Types

The unique_ptr equivalent: box<A> | unit
```C++
opt<box<A>>
```

The weak_ptr equivalent: rc<A> | unit
```C++
opt<rc<A>>
```
