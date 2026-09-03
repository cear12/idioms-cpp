// Named External Argument idiom: each parameter gets its own tag type and
// factory function, so a call site reads like named arguments and
// swapping two same-typed arguments becomes a type error instead of a
// silent bug.
#include <iostream>
#include <string>

struct Width {
  int value_;
};
struct Height {
  int value_;
};
struct Resizable {
  bool value_;
};
struct Decorated {
  bool value_;
};
struct Title {
  std::string value_;
};

constexpr Width MakeWidth(int v) { return Width{v}; }
constexpr Height MakeHeight(int v) { return Height{v}; }
constexpr Resizable MakeResizable(bool v) { return Resizable{v}; }
constexpr Decorated MakeDecorated(bool v) { return Decorated{v}; }
Title MakeTitle(const std::string& v) { return Title{v}; }

void ConfigureWindow(Width w, Height h, Resizable r, Decorated d, Title t) {
  std::cout << "Window Configuration:\n"
            << "  Width     = " << w.value_ << "\n"
            << "  Height    = " << h.value_ << "\n"
            << "  Resizable = " << std::boolalpha << r.value_ << "\n"
            << "  Decorated = " << std::boolalpha << d.value_ << "\n"
            << "  Title     = " << t.value_ << "\n";
}

int main() {
  // Reads like named arguments; swapping MakeWidth(...)/MakeHeight(...) by
  // accident would be caught at compile time since they're distinct types.
  ConfigureWindow(MakeWidth(800), MakeHeight(600), MakeResizable(true),
                  MakeDecorated(false), MakeTitle("My App Window"));
  return 0;
}
