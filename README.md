## Welcome to libCat

This is a C++17 library which implements concepts from the Category Theory.
The C++ standard library partialy does this, but we put it in a different view.
The main objective is to enforce the programmer to write correct code,
with lesser undefined behaviour.

### Algebraic Types

```C++
Product types:
all<A, B, ...> ~ Haskell (A, B, ...)
all<> = nil ~ Haskell ()
Sum types:
any<A, B, ...> ~ Haskell A | B | ...
any<> = nvr ~ Haskell Void

```

### Memory Management Types

```C++

box<A> ~ C++ std::unique_ptr<A>, but never nullptr
rc<A> ~ C++ std::shared_ptr<A>, but never nullptr

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

