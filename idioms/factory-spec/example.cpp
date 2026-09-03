// Factory with Specification Objects idiom: a BuildingSpec hierarchy
// parallels the Building hierarchy, carrying each concrete type's own
// construction parameters, so one factory function can build any of them
// from a single `const BuildingSpec*`.
#include <iostream>

enum class Buildings { FARM, FORGE };

// --- Specifications: one concrete spec per concrete Building, each
// carrying that type's own construction parameters.
struct BuildingSpec {
    virtual Buildings building_type() const = 0;
    virtual ~BuildingSpec() = default;
};

struct FarmSpec : BuildingSpec {
    Buildings building_type() const override { return Buildings::FARM; }
    bool withPasture = false;
    int numberOfStalls = 0;
};

struct ForgeSpec : BuildingSpec {
    Buildings building_type() const override { return Buildings::FORGE; }
    bool magicForge = false;
    int numberOfApprentices = 0;
};

// --- Buildings: each concrete type is constructed from its matching spec.
class Building {
public:
    virtual ~Building() = default;
    virtual void describe() const = 0;
};

class Farm : public Building {
public:
    explicit Farm(const FarmSpec& spec)
        : withPasture_(spec.withPasture), numberOfStalls_(spec.numberOfStalls) {}

    void describe() const override {
        std::cout << "Farm: pasture=" << std::boolalpha << withPasture_
                  << ", stalls=" << numberOfStalls_ << "\n";
    }

private:
    bool withPasture_;
    int numberOfStalls_;
};

class Forge : public Building {
public:
    explicit Forge(const ForgeSpec& spec)
        : magicForge_(spec.magicForge), numberOfApprentices_(spec.numberOfApprentices) {}

    void describe() const override {
        std::cout << "Forge: magic=" << std::boolalpha << magicForge_
                  << ", apprentices=" << numberOfApprentices_ << "\n";
    }

private:
    bool magicForge_;
    int numberOfApprentices_;
};

// --- The factory: dispatches on building_type(), then downcasts the spec
// to the matching concrete type before forwarding it to that type's
// constructor.
Building* MakeBuilding(const BuildingSpec& spec) {
    switch (spec.building_type()) {
        case Buildings::FARM:
            return new Farm(static_cast<const FarmSpec&>(spec));
        case Buildings::FORGE:
            return new Forge(static_cast<const ForgeSpec&>(spec));
    }
    return nullptr;
}

int main() {
    FarmSpec farmSpec;
    farmSpec.withPasture = true;
    farmSpec.numberOfStalls = 4;

    ForgeSpec forgeSpec;
    forgeSpec.magicForge = true;
    forgeSpec.numberOfApprentices = 2;

    Building* farm = MakeBuilding(farmSpec);
    Building* forge = MakeBuilding(forgeSpec);

    farm->describe();
    forge->describe();

    delete farm;
    delete forge;
    return 0;
}
