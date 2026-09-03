// Construction Tracker idiom: a function-try-block around the whole
// constructor catches exceptions from the member-initializer list, and
// each tracked member updates `tracker` as a side effect of its own
// construction, so the catch block knows which member was being built
// when construction failed.
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

namespace construction_tracker {

class Tracker {
public:
    enum TrackerType { TrackNone, TrackA, TrackB, TrackCount };

    Tracker()
    try
        : trackA_((tracker_ = TrackA, 0))            // may throw
        , trackB_((tracker_ = TrackB, std::string())) // may throw
    {
        tracker_ = TrackCount; // reached only if both members succeeded
        std::cout << "Tracker fully constructed\n";
    } catch (...) {
        std::cout << "Construction failed while building step "
                  << tracker_ << " of " << TrackCount << "\n";
        throw; // function-try-blocks on constructors must rethrow (or the
               // standard turns falling off the end into std::terminate)
    }

private:
    // Named StepA/StepB (not TrackA/TrackB) to avoid colliding with this
    // class's own TrackA/TrackB *enumerators* above, which are injected
    // into this same class scope by the unscoped enum.
    struct StepA {
        explicit StepA(int) { /* nothing to do */ }
    };
    struct StepB {
        explicit StepB(std::string) { throw std::runtime_error("simulated failure in StepB"); }
    };

    TrackerType tracker_ = TrackNone;
    StepA trackA_;
    StepB trackB_;
};

} // namespace construction_tracker

int main() {
    using namespace construction_tracker;
    try {
        Tracker t;
        (void)t;
    } catch (const std::exception& e) {
        std::cout << "Caught in main: " << e.what() << "\n";
    }
    return 0;
}
