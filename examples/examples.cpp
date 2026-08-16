#include <iostream>

#include "ecx/core/coordinator.hpp"

using namespace ecx;

struct Transform
{
    float x = 0.0f;
    float y = 0.0f;
};

int main()
{
    Coordinator coordinator;

    // Registering up front is optional (add() will auto-register), but it's
    // the place to set a custom initial_capacity if you know roughly how
    // many entities will carry this component.
    coordinator.register_component_type<Transform>();

    Entity player = coordinator.create_entity();
    Entity enemy = coordinator.create_entity();

    coordinator.add_component<Transform>(player, Transform{0.0f, 0.0f});
    coordinator.add_component<Transform>(enemy, Transform{10.0f, 5.0f});

    if (coordinator.has_component<Transform>(player))
    {
        Transform *t = coordinator.get_component<Transform>(player);
        std::cout << "Player at (" << t->x << ", " << t->y << ")\n";
    }

    // get_component returns a live pointer into storage, so this mutates in place
    if (Transform *t = coordinator.get_component<Transform>(player))
    {
        t->x += 1.0f;
        t->y += 1.0f;
    }

    std::cout << "Player moved to ("
              << coordinator.get_component<Transform>(player)->x << ", "
              << coordinator.get_component<Transform>(player)->y << ")\n";

    // Removals are deferred until flush()
    coordinator.remove_component<Transform>(enemy);
    coordinator.flush();

    std::cout << "Enemy still has Transform? " << std::boolalpha
              << coordinator.has_component<Transform>(enemy) << "\n";

    // Destroying an entity also cleans up all of its components
    coordinator.destroy_entity(player);
    coordinator.flush();

    std::cout << "Player still alive? " << std::boolalpha
              << coordinator.is_entity_alive(player) << "\n";

    // Wipe the whole world and start clean
    coordinator.reset();
    coordinator.flush();

    return 0;
}
