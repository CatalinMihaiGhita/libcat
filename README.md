## Welcome to libCat

This is a C++17 library which implements concepts from the Category Theory.
The C++ standard library partialy does this, but we put it in a different view.
The main objective is to enforce the programmer to write correct code,
with lesser undefined behaviour.

### Algebraic Types

#### Product types
The tuple type (A, B, ...)
```C++
all<A, B, ...>
```
The unit type ()
```C++
nil = all<>
```

#### Sum types
The variant type A | B | ...
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

### Monad Types

```C++
opt<A> = A | nil
lzy<A> = A | nevr
vec<A> = (A, ...) | nil

```

### Monad Types + Memory Management Types

```C++
opt<box<A>> = box<A> | nil ~ C++ std::unique_ptr<A>
opt<rc<A>> = rc<A> | nil ~ C++ std::shared_ptr<A>

```

