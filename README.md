## Welcome to libCat

This is a C++17 library which implements concepts from the Category Theory.
The C++ standard library partialy does this, but we put it in a different view.
The main objective is to enforce the programmer to write correct code,
with lesser undefined behaviour.

### Algebraic Types

```C++

all<A, B, ...> ~ (A, B, ...)
all<> = nil ~ ()
any<A, B, ...> ~ A | B | ...
any<> = nevr ~ Void

```

### Memory Management Types

```C++

box<A> ~ std::unique_ptr<A>, but never nullptr
rc<A> ~ std::shared_ptr<A>, but never nullptr

```

### Monad Types

```C++

opt<A> = A | nil
lzy<A> = A | nevr
vec<A> = (A, ...) | nil

```

### Monad Types + Memory Management Types

```C++

opt<box<A>> = box<A> | nil ~ std::unique_ptr<A>
opt<rc<A>> = rc<A> | nil ~ std::shared_ptr<A>

```

