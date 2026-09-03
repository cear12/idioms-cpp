// Factory with Specification Objects idiom: a BuildingSpec hierarchy
// parallels the Building hierarchy, carrying each concrete type's own
// construction parameters, so one factory function can build any of them
// from a single `const BuildingSpec*`.
#include <iostream>

enum class Buildings { kFarm, kForge };

// --- Specifications: one concrete spec per concrete Building, each
// carrying that type's own construction parameters.
struct BuildingSpec {
    virtual Buildings BuildingType() const = 0;
    virtual ~BuildingSpec() = default;
};

struct FarmSpec : BuildingSpec {
    Buildings BuildingType() const override { return Buildings::kFarm; }
    bool with_pasture_ = false;
    int number_of_stalls_ = 0;
};

struct ForgeSpec : BuildingSpec {
    Buildings BuildingType() const override { return Buildings::kForge; }
    bool magic_forge_ = false;
    int number_of_apprentices_ = 0;
};

// --- Buildings: each concrete type is constructed from its matching spec.
class Building {
public:
    virtual ~Building() = default;
    virtual void Describe() const = 0;
};

class Farm : public Building {
public:
    explicit Farm(const FarmSpec& spec)
        : with_pasture_(spec.with_pasture_), number_of_stalls_(spec.number_of_stalls_) {}

    void Describe() const override {
        std::cout << "Farm: pasture=" << std::boolalpha << with_pasture_
                  << ", stalls=" << number_of_stalls_ << "\n";
    }

private:
    bool with_pasture_;
    int number_of_stalls_;
};

class Forge : public Building {
public:
    explicit Forge(const ForgeSpec& spec)
        : magic_forge_(spec.magic_forge_), number_of_apprentices_(spec.number_of_apprentices_) {}

    void Describe() const override {
        std::cout << "Forge: magic=" << std::boolalpha << magic_forge_
                  << ", apprentices=" << number_of_apprentices_ << "\n";
    }

private:
    bool magic_forge_;
    int number_of_apprentices_;
};

// --- The factory: dispatches on BuildingType(), then downcasts the spec
// to the matching concrete type before forwarding it to that type's
// constructor.
Building* MakeBuilding(const BuildingSpec& spec) {
    switch (spec.BuildingType()) {
        case Buildings::kFarm:
            return new Farm(static_cast<const FarmSpec&>(spec));
        case Buildings::kForge:
            return new Forge(static_cast<const ForgeSpec&>(spec));
    }
    return nullptr;
}

int main() {
    FarmSpec farm_spec;
    farm_spec.with_pasture_ = true;
    farm_spec.number_of_stalls_ = 4;

    ForgeSpec forge_spec;
    forge_spec.magic_forge_ = true;
    forge_spec.number_of_apprentices_ = 2;

    Building* farm = MakeBuilding(farm_spec);
    Building* forge = MakeBuilding(forge_spec);

    farm->Describe();
    forge->Describe();

    delete farm;
    delete forge;
    return 0;
}
