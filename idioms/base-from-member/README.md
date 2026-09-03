# Base-from-Member

## Intent

Ensure a data member is fully constructed before it is passed to a base
class's constructor, when the base class needs that member as a
construction argument.

## Motivation

Base classes are always constructed before members, regardless of the
order they're written in a derived class's member-initializer list. If a
base class constructor needs to receive one of the derived class's own
members (a common need when the member is, say, a resource the base
wraps), naively passing that member from the initializer list uses it
*before* it has been constructed -- undefined behavior, and exactly the
kind of bug that's easy to write and hard to notice:

```cpp
struct Derived : Base {
    Member member;
    Derived(int x)
        : Base(member),  // BUG: `member` is not constructed yet here
          member(x)
    {}
};
```

## Solution

Insert an intermediate base class, `base_from_member<MemberType>`, ahead
of `Base` in the inheritance list. Because base classes are constructed in
declaration (inheritance-list) order, `base_from_member`'s own `member` is
fully constructed *before* `Base` is, so `Base`'s constructor can safely
receive a reference to it.

## Known Uses

- Any base class whose constructor needs a reference to something the
  derived class also wants to expose as its own member, e.g. a `Base`
  that wraps a `std::mutex&` also kept as a derived-class member.
- `std::basic_ios`-style designs in parts of the standard library face
  this exact ordering problem.

## References

- [More C++ Idioms - Base-from-Member](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Base-from-Member)
