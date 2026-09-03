# Thread-Safe Interface

## Intent

Expose an API where every public operation is safe to call concurrently
from multiple threads, by encapsulating all synchronization inside the
class rather than requiring callers to manage locking themselves.

## Motivation

Data races and undefined behavior happen when multiple threads touch
shared mutable state without coordination. Pushing the locking discipline
onto every caller of a shared container is fragile — one missed lock
anywhere corrupts state or crashes the program. The Thread-Safe Interface
idiom instead wraps the shared resource in a class whose every public
method takes the necessary lock internally, so callers just use it like
any ordinary (single-threaded) object.

## Solution

`ThreadSafeVector<T>` wraps a `std::vector<T>` and a `std::mutex`; every
public method takes a `std::lock_guard` before touching the underlying
vector. Callers never see or manage the mutex directly.

## Use Cases

- Shared containers, counters, caches, and pools accessed by multiple
  threads in a service.
- Logging objects and other resources with concurrent readers and writers.
- Libraries that must guarantee correctness under concurrent use without
  requiring callers to understand the locking scheme.

## References

- [More C++ Idioms - Thread-Safe Interface](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Thread-Safe_Interface)
