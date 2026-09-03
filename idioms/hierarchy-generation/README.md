# Hierarchy Generation

## Intent

Generate a concrete class composed of an arbitrary number of behavioral
policies by chaining single inheritance, instead of reaching for multiple
inheritance.

## Motivation

Combining independent behaviors (dance style, joke style, song style, ...)
via multiple inheritance runs into the usual MI headaches: diamond
ambiguities, base-class ordering, and a combinatorial explosion of concrete
classes if done by hand. The Hierarchy Generation idiom instead builds a
single linear chain of inheritance at compile time: each policy template
derives from the "rest of the chain", so the final type is one class with
one (long) line of single-inheritance ancestors.

## Solution

`GenHierarchy<P1, P2, ..., Pn>` recursively expands to
`P1<P2<...<Pn<GenHierarchy<>>>...>>`, i.e. `P1` inherits from `P2<...>`,
which inherits from `P3<...>`, and so on down to the empty terminator. Each
`Px` template contributes one method to the final type. Combining policies
is then just naming them in a template argument list.

## Known Uses

- Generating many concrete types from a small set of orthogonal behavior
  policies without hand-writing every combination.
- Policy-based design (see the sibling `policy-based-design` idiom) when
  more than a couple of independent policies need to compose.

## Related Idioms

- Policy-Based Design, CRTP.

## References

- [More C++ Idioms - Hierarchy Generation](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Hierarchy_Generation)
