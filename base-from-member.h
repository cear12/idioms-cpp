struct Member {
    Member(int x) { /* ... */ }
};

struct Base {
    Base(Member& m) { /* ... */ }
};

struct Derived : Base {
    Member member;
    Derived(int x) 
        : Base(member),  // ОШИБКА: `member` ещё не инициализирован!
        member(x)        // member создаётся только здесь
    {}
};

template <typename MemberType, int UniqueID = 0>
struct base_from_member {
    MemberType member;

    // Perfect forwarding для конструктора
    template <typename... Args>
    base_from_member(Args&&... args) 
        : member(std::forward<Args>(args)...) 
    {}
};

struct Derived : private base_from_member<Member>, public Base {
    Derived(int x) 
        : base_from_member<Member>(x),  // сначала создаётся `member`
          Base(member)                  // затем передаётся в `Base`
    {}
};