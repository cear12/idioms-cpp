# Weak This

## Intent

Let an object safely schedule a callback (async I/O, a timer, a task
queue) that refers back to itself, without that pending callback keeping
the object alive past its natural lifetime, and without the callback
touching a destroyed object if it fires after the object is gone.

## Motivation

Capturing `this` by raw pointer in an async callback is dangerous: if the
object is destroyed before the callback fires, the callback dereferences a
dangling pointer. Capturing `shared_from_this()` (a `shared_ptr`) instead
keeps the object alive artificially just because a callback is pending,
which is often not what's wanted and can create surprising lifetime
extension or reference cycles. Capturing a `weak_ptr` to `this` gets both
properties right: it doesn't keep the object alive, and the callback can
safely check whether the object is still alive before touching it.

## Solution

The class inherits `std::enable_shared_from_this<T>` and hides its
constructor behind a `create()` factory that immediately wraps it in a
`shared_ptr` -- `shared_from_this()`/`weak_from_this()` are only valid on
objects owned by a `shared_ptr`, so making the plain constructor private
enforces that at compile time. Any method that schedules an async
callback captures `weak_from_this()`, not `this`; the callback calls
`.lock()` when it eventually runs, and only proceeds if that succeeds.

## Known Uses

- Any object that hands a callback to something outside its own control
  (an event loop, a thread pool, a network layer) and needs that callback
  to be a safe no-op instead of a dangling-pointer bug if the object is
  gone by the time the callback fires.

## References

- [cppreference - std::enable_shared_from_this](https://en.cppreference.com/w/cpp/memory/enable_shared_from_this)
- [cppreference - std::weak_ptr](https://en.cppreference.com/w/cpp/memory/weak_ptr)
