## Welcome to libCat

This is a C++17 library which implements concepts from the Category Theory.
The C++ standard library partialy does this, but we put it in a different view.
The main objective is to enforce the programmer to write correct code,
with lesser undefined behaviour.

### Algebraic Types

#### Product types
The tuple type: (A, B, ...)
```C++
all<A, B, ...>
```
The unit type ()
```C++
nil = all<>
```

#### Sum types
The variant type: A | B | ...
```C++
any<A, B, ...>
```
The zero type
```C++
nvr = any<>
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
lnk<A>
```

### Monad Types

The optional type: A | nil
```C++
opt<A> = any<A, nil> 
```

The lazy type: A | nvr
```C++
lzy<A> = any<A, nvr>
```

The vector type (A, ...) | nil
```C++
vec<A>
```

### Monad Types + Memory Management Types

The unique_ptr equivalent: box<A> | nil
```C++
opt<box<A>>
```

The weak_ptr equivalent: rc<A> | nil
```C++
wk<A> = opt<rc<A>>
```
