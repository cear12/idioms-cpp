# idioms-cpp

A collection of 30 C++ idioms, each with a short write-up and a
self-contained, compilable, runnable example -- built and exercised as
part of CI rather than left as unverified notes.

## Structure

```
idioms/
  <idiom-name>/
    README.md      # Intent / Motivation / Solution / Known Uses / References
    example.cpp    # self-contained example with its own main()
```

Every `example.cpp` is deliberately independent of every other one (no
shared header, unlike this author's `leetcode-cpp` repository) -- these
are meant to be readable and copy-pasteable in isolation, which is also
how the idiom is normally encountered in the wild.

## Building and running

```sh
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

Every `example.cpp` becomes its own executable and its own CTest case (30
in total). There are no external dependencies beyond a C++17 compiler and
threading support (`thread-safe-interface` uses `std::thread`).

## Building in Visual Studio

With 30 independent idiom executables and no single "main app", Visual
Studio's Open Folder / CMake integration has nothing to pick as a default
startup item. Without one, pressing **Debug/Run** (not Build) pops a
blocking "Select Startup Item" dialog -- easy to mistake for the project
failing to build, even though **Build > Build All** (Ctrl+Shift+B)
succeeds regardless of what's selected there. `CMakePresets.json` sets
`CMAKE_VS_STARTUP_PROJECT` to `acyclic-visitor` (alphabetically first) so
Debug/Run works immediately too; pick a different target from the
dropdown next to the Run button to debug any of the others.

## Index

### Construction & lifetime

- [`base-from-member`](idioms/base-from-member) -- construct a member before it's passed to a base class constructor.
- [`construction-tracker`](idioms/construction-tracker) -- know which member was being built when a constructor throws, via a function-try-block.
- [`fast-pimpl`](idioms/fast-pimpl) -- Pimpl without the heap allocation, using inline fixed-size storage.
- [`no-heap-class`](idioms/no-heap-class) -- make heap-allocating a type a compile error.
- [`weak-this`](idioms/weak-this) -- capture a safe, non-owning self-reference in an async callback.

### Polymorphism & dispatch

- [`acyclic-visitor`](idioms/acyclic-visitor) -- Visitor pattern without forcing every visitor to know every element type.
- [`covariant-return-types`](idioms/covariant-return-types) -- an override narrowing its return type to a more derived pointer/reference.
- [`crtp`](idioms/crtp) -- static (compile-time) polymorphism via the Curiously Recurring Template Pattern.
- [`fake-vtable`](idioms/fake-vtable) -- hand-rolled single dispatch via a function-pointer member instead of `virtual`.
- [`lambda-visitor`](idioms/lambda-visitor) -- build a `std::visit` visitor via a fold expression over a lambda pack.
- [`overload-visit-variant`](idioms/overload-visit-variant) -- build a `std::visit` visitor by aggregating lambdas via multiple inheritance.

### Casting & type identity

- [`downcast`](idioms/downcast) -- five downcasting techniques compared side by side (`dynamic_cast`, virtual is-a, type tags, manual RTTI, `constexpr`-branching).
- [`manual-rtti`](idioms/manual-rtti) -- hand-rolled dynamic type identification without `typeid`/`dynamic_cast`.
- [`type-erasure`](idioms/type-erasure) -- store any copyable type behind one non-template type (a minimal `std::any`).
- [`universal-ptr`](idioms/universal-ptr) -- one pointer type that can either own or merely observe, depending on how it's constructed.
- [`implicit-conversion`](idioms/implicit-conversion) -- converting constructors and conversion operators, and what `explicit` suppresses.

### Templates & generic programming

- [`hierarchy-generation`](idioms/hierarchy-generation) -- compose behavior policies via chained single inheritance instead of multiple inheritance.
- [`policy-based-design`](idioms/policy-based-design) -- configure a class's behavior via template parameters instead of virtual functions.
- [`named-template-parameter`](idioms/named-template-parameter) -- pass template arguments by name/tag, in any order, with defaults.
- [`named-external-argument`](idioms/named-external-argument) -- make ordinary function call sites self-documenting via tag types.
- [`shortening-long-template-names`](idioms/shortening-long-template-names) -- `typedef`, `using`, and alias templates for unwieldy instantiations.
- [`sfinae`](idioms/sfinae) -- detect at compile time whether an expression is well-formed for a given type.
- [`result-of-technique`](idioms/result-of-technique) -- deduce a callable's return type (`std::result_of`, and its replacement `std::invoke_result_t`).
- [`std-function-implementation`](idioms/std-function-implementation) -- a minimal `std::function` built from first principles.

### Concurrency

- [`thread-safe-interface`](idioms/thread-safe-interface) -- encapsulate all synchronization inside a class's public API.

### Functions, macros & miscellany

- [`do-while-0-macro`](idioms/do-while-0-macro) -- make a multi-statement macro behave like a single statement.
- [`function-poising`](idioms/function-poising) -- turn an accidental call to a deprecated function into a compile error.
- [`factory-spec`](idioms/factory-spec) -- a factory function driven by a hierarchy of specification objects.
- [`exploding-return-type`](idioms/exploding-return-type) -- a return value that throws on use if it actually represents an error.
- [`nullptr-emulation`](idioms/nullptr-emulation) -- the pre-C++11 library emulation of what became the `nullptr` keyword.

## Notes on this collection

Several of these idioms started life as design notes or incomplete
sketches rather than working code (missing includes, undefined behavior,
or template metaprogramming that didn't actually do what its own comments
claimed). Bringing all 30 up to compiling, warning-free, and independently
runnable surfaced a number of real bugs and design gaps beyond
formatting -- see each idiom's own `README.md` for idiom-specific notes,
and the top-level portfolio report for a consolidated list.

## CI

`.github/workflows/ci.yml` builds and runs the full CTest suite on a
Linux/macOS/Windows matrix on every push and pull request.
