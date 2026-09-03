# Factory with Specification Objects

## Intent

Build objects of different concrete types from a single factory function,
where each type also needs its own distinct set of construction
parameters -- without the factory function's signature having to know
about every concrete type's parameter list.

## Motivation

A classic factory function like `Building* MakeBuilding(Buildings kind)`
works when every concrete type can be default-constructed, but falls
apart once different concrete types need different construction data (a
farm needs a stall count, a forge needs an apprentice count). Widening the
factory's own parameter list to cover every concrete type's needs doesn't
scale and leaks unrelated types' details into the factory's interface.

## Solution

Introduce a `BuildingSpec` hierarchy parallel to the `Building` hierarchy:
each concrete spec (`FarmSpec`, `ForgeSpec`) carries exactly the
construction data its matching concrete `Building` needs, and identifies
its own kind via a virtual `BuildingType()`. The factory function takes a
single `const BuildingSpec*`, switches on `BuildingType()`, and
`static_cast`s to the matching concrete spec type before forwarding its
fields to the matching concrete `Building` constructor.

## Known Uses

- Deserializing polymorphic objects from heterogeneous configuration data
  (each concrete type's spec mirrors its own config schema).
- Game object factories where different unit/building types are
  constructed from different designer-authored parameter sets.

## Related Idioms

- Abstract Factory (GoF) solves a related but different problem: producing
  families of related objects, not objects configured by heterogeneous
  parameter sets.

## References

- [Factory method pattern (Wikipedia)](https://en.wikipedia.org/wiki/Factory_method_pattern)
