#include "ecx/core/coordinator.hpp"

namespace ecx
{
    // Entity
    Entity Coordinator::create_entity()
    {
        return m_entity_manager.create();
    }

    bool Coordinator::is_entity_alive(Entity entity) const
    {
        return m_entity_manager.is_alive(entity);
    }

    void Coordinator::destroy_entity(Entity entity)
    {
        m_entity_manager.destroy(entity);
        m_component_manager.remove_all_by_entity(entity);
    }

    void Coordinator::destroy_all_entities()
    {
        m_entity_manager.destroy_all();
        m_component_manager.remove_all();
    }

    // Component
    void Coordinator::remove_all_components_by_entity(Entity entity)
    {
        m_component_manager.remove_all_by_entity(entity);
    }

    // Shared
    void Coordinator::flush()
    {
        m_entity_manager.flush();
        m_component_manager.flush();
    }

    void Coordinator::reset()
    {
        m_entity_manager.reset();
        m_component_manager.reset();
    }
}
