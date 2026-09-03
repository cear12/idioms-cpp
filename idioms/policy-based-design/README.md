# Policy-Based Design

## Intent

Assemble a class's behavior from independent, interchangeable template
parameters ("policies") instead of virtual functions, so the combination
is chosen and resolved entirely at compile time.

## Motivation

Runtime polymorphism (virtual functions) is one way to make a container or
algorithm configurable, but it costs an indirect call and forces all
strategies through a common virtual interface. When the strategy is known
at compile time, template parameters let the compiler inline and optimize
across the "seam" instead, and let each policy have a completely
independent interface.

## Solution

`Container<T, AllocPolicy, LogPolicy>` privately inherits from both policy
templates and calls their methods directly (`this->allocate(...)`,
`this->log(...)`); swapping behavior is just picking different template
arguments, with no vtable involved anywhere.

## Applicability

- Libraries and frameworks needing flexible configuration without virtual
  functions.
- Performance-sensitive code that must avoid virtual call overhead.
- Containers and algorithms whose behavioral aspects (allocation, locking,
  logging, ...) can be factored into independent policies.

## Related Idioms

- Hierarchy Generation (for composing more than a couple of policies).
- CRTP (policies are frequently implemented as CRTP bases).

## References

- [More C++ Idioms - Policy Clone](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Policy_Clone)
- Alexandrescu, *Modern C++ Design* (the idiom's canonical reference).
