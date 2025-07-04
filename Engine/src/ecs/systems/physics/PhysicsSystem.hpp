#pragma 
#include "../BaseSystem.hpp"

struct PhysicsSystem : public BaseSystem
{
    void update(double delta);

    void onEntityAdded(EntityID e) override;
    void onEntityRemoved(EntityID e) override;

private:
    const uint8_t meter = 32; // 1 meter = 32px

    std::vector<EntityID> entities;
};

