# Fast Pimpl

## Intent

Get the Pimpl (pointer-to-implementation) idiom's compile-time firewall
benefit (changing the implementation doesn't force clients to recompile)
without its usual extra heap allocation, by storing the implementation
object inline in a fixed-size byte buffer instead of behind a
separately-allocated pointer.

## Motivation

Ordinary Pimpl (`Base -> Impl*`, with `Impl` heap-allocated in the
constructor) hides implementation details behind an incomplete type, but
pays for a heap allocation and an extra pointer indirection on every
access. If the implementation type's size is known to stay within some
fixed bound, Fast Pimpl reserves that many bytes directly inside the
public class and constructs the (still-incomplete-at-the-point-of-
declaration) implementation type into that buffer with placement `new` --
no separate allocation, and the implementation object lives directly
inside the public object's own storage.

## Solution

Reserve a suitably aligned, fixed-size `char` buffer (`alignas(...) char
storage[N];`) in the public class. In the `.cpp` file, where the
implementation type is complete, `static_assert(sizeof(Impl) <=
sizeof(storage))` catches at compile time if the buffer is too small;
placement `new (storage) Impl(...)` constructs the implementation object
in place, and the destructor explicitly calls `~Impl()`.

## Trade-offs

The public header must reserve *some* fixed capacity for a type it can't
see the size of yet -- too small and the `static_assert` in the `.cpp`
file fails (loudly, at compile time, which is the safe failure mode);
too large and the public class wastes space. This is the price paid for
avoiding the heap allocation ordinary Pimpl accepts unconditionally.

## References

- [More C++ Idioms - Fast Pimpl](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Fast_Pimpl)
