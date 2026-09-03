// Weak This idiom: ImageView::startLoad captures weak_from_this() instead
// of `this` (or a shared_ptr) in its "async" callback, so the callback
// neither keeps the ImageView alive nor dereferences it after it's gone.
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

class ImageView : public std::enable_shared_from_this<ImageView> {
public:
    // Constructor is private; the only way to get an ImageView is through
    // create(), which guarantees it's always owned by a shared_ptr --
    // required for shared_from_this()/weak_from_this() to be valid.
    static std::shared_ptr<ImageView> Create(int id) {
        return std::shared_ptr<ImageView>(new ImageView(id));
    }

    // Simulates kicking off an async load: the "network layer" here is
    // just a vector of callbacks main() will invoke later, standing in for
    // whatever a real event loop or I/O completion handler would call.
    void StartLoad(std::vector<std::function<void()>>& pending_callbacks) {
        std::weak_ptr<ImageView> weak_self = weak_from_this();
        pending_callbacks.push_back([weak_self]() {
            if (auto self = weak_self.lock()) {
                self->OnLoaded();
            } else {
                std::cout << "Callback fired after its ImageView was destroyed; skipped safely\n";
            }
        });
    }

    void OnLoaded() const {
        std::cout << "ImageView " << id_ << " finished loading\n";
    }

private:
    explicit ImageView(int id) : id_(id) {}
    int id_;
};

int main() {
    std::vector<std::function<void()>> pending;

    auto long_lived = ImageView::Create(1);
    long_lived->StartLoad(pending);

    {
        auto short_lived = ImageView::Create(2);
        short_lived->StartLoad(pending);
    } // short_lived destroyed here; its pending callback's weak_ptr will be expired

    std::cout << "-- running pending callbacks --\n";
    for (auto& cb : pending) cb();

    return 0;
}
