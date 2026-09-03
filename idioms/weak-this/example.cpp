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
    static std::shared_ptr<ImageView> create(int id) {
        return std::shared_ptr<ImageView>(new ImageView(id));
    }

    // Simulates kicking off an async load: the "network layer" here is
    // just a vector of callbacks main() will invoke later, standing in for
    // whatever a real event loop or I/O completion handler would call.
    void startLoad(std::vector<std::function<void()>>& pendingCallbacks) {
        std::weak_ptr<ImageView> weakSelf = weak_from_this();
        pendingCallbacks.push_back([weakSelf]() {
            if (auto self = weakSelf.lock()) {
                self->onLoaded();
            } else {
                std::cout << "Callback fired after its ImageView was destroyed; skipped safely\n";
            }
        });
    }

    void onLoaded() const {
        std::cout << "ImageView " << id_ << " finished loading\n";
    }

private:
    explicit ImageView(int id) : id_(id) {}
    int id_;
};

int main() {
    std::vector<std::function<void()>> pending;

    auto longLived = ImageView::create(1);
    longLived->startLoad(pending);

    {
        auto shortLived = ImageView::create(2);
        shortLived->startLoad(pending);
    } // shortLived destroyed here; its pending callback's weak_ptr will be expired

    std::cout << "-- running pending callbacks --\n";
    for (auto& cb : pending) cb();

    return 0;
}
