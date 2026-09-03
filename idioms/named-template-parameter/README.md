# Named Template Parameter

## Intent

Let callers of a template specify only the template parameters they want
to override, by name, in any order, while the rest fall back to defaults
-- the template-parameter analogue of the Named External Argument idiom.

## Motivation

A template with several policy-like parameters
(`template<typename T, typename Cmp, std::size_t N> class Container`) is
awkward to configure when a caller wants to change, say, only `N`: they
must still spell out `T` and `Cmp` explicitly (or rely on them being
defaulted and the parameter they want being last). Named template
parameters let each parameter be wrapped in a small "this is the `Size`
one" tag type and passed in any order; a small metafunction sorts them out
by tag rather than by position.

## Solution

Each wrapper type (`TypeParam<T>`, `ComparatorParam<Cmp>`,
`SizeParam<N>`) carries its own `Tag` type. `find_param<Tag, Default,
Params...>` walks the parameter pack looking for the wrapper whose `Tag`
matches, and falls back to a supplied default if none is found.
`apply_named_params` uses `find_param` once per logical parameter and
feeds the results to the target template positionally -- so from the
caller's side, order never matters, only which wrapper type was used.

## Known Uses

- Configuring a template with many optional/defaultable parameters (this
  is essentially how Boost.Parameter and similar "named template
  arguments" libraries work, generalized).

## Related Idioms

- Named External Argument (the same idea, one level up, for ordinary
  function arguments instead of template arguments).
- Policy-Based Design.

## References

- [More C++ Idioms - Named Template Parameters](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Named_Template_Parameters)
