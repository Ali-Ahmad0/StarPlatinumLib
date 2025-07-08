#pragma 
#include "../BaseSystem.hpp"

struct PhysicsSystem : public BaseSystem
{
    void update(double delta);

    void onEntityAdded(EntityID e) override;
    void onEntityRemoved(EntityID e) override;

private:
    std::vector<EntityID> entities;
};

