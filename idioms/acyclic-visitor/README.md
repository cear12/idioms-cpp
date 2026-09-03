# Acyclic Visitor

## Intent

Implement the Visitor pattern without forcing every visitor to know about
every visitable type, and without a cyclic compile-time dependency between
the visitor hierarchy and the visited hierarchy.

## Motivation

The classical (GoF) Visitor pattern declares one `Visit(ElementX&)` method
per element type on a single `Visitor` base class, which every concrete
visitor must implement (or stub out) -- adding a new element type means
touching every existing visitor, and the `Visitor` base and the element
hierarchy depend on each other cyclically at compile time. The Acyclic
Visitor idiom breaks this by giving each element type its own tiny
single-method interface, `VisitorFor<ElementX>`, and having concrete
visitors implement only the ones they actually care about.

## Solution

`Visitable::Accept(Visitor&)` `dynamic_cast`s the passed-in `Visitor&` to
`VisitorFor<ThisElementType>*`; if the concrete visitor implements that
interface, the cast succeeds and the type-specific `Visit()` is called, if
not, the element is silently skipped by that visitor. Adding a new element
type only requires a new `VisitorFor<NewElement>` interface -- existing
visitors that don't implement it keep compiling unchanged.

## Advantages

No existing `Visitor` classes need modification when a new visitable type
is added. Visitors are not forced to implement methods for types they
don't care about. This simplifies development of the object hierarchy and
traversal logic, at the cost of a `dynamic_cast` (and its RTTI dependency)
per `Accept()` call.

## Known Uses

Complex systems where the set of types that can be visited grows over time
and no single visitor needs to handle all of them, e.g. parsers, ASTs, and
GUI systems.

## References

- [More C++ Idioms - Acyclic Visitor](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Acyclic_Visitor)
