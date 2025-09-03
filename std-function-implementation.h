struct ICallable {
    virtual R invoke(args...) = 0;
    virtual ~ICallable() = default;
};

template <typename F>
struct CallableImpl : ICallable {
    F f;
    CallableImpl(F&& _f) : f(std::forward<F>(_f)) {}
    R invoke(args...) override { return f(args...); }
};