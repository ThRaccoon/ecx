#pragma once

#include "ecx/core/types.hpp"
#include "ecx/entity/entity_manager.hpp"
#include "ecx/component/component_manager.hpp"

namespace ecx
{
    class Coordinator
    {
    public:
        // Entity
        Entity create_entity();
        bool is_entity_alive(Entity entity) const;
        void destroy_entity(Entity entity);
        void destroy_all_entities();

        // Component
        template <typename Component>
        void register_component_type(std::size_t initial_capacity = internal::SparseSet<Entity>::DEFAULT_INITIAL_CAPACITY)
        {
            m_component_manager.register_type<Component>(initial_capacity);
        }

        template <typename Component>
        void add_component(Entity entity, Component component)
        {
            m_component_manager.add<Component>(entity, component);
        }

        template <typename Component>
        bool has_component(Entity entity)
        {
            return m_component_manager.has<Component>(entity);
        }

        template <typename Component>
        Component *get_component(Entity entity)
        {
            return m_component_manager.get<Component>(entity);
        }

        template <typename Component>
        void remove_component(Entity entity)
        {
            m_component_manager.remove<Component>(entity);
        }

        // Shared
        void flush();
        void reset();

    private:
        internal::EntityManager m_entity_manager;
        internal::ComponentManager m_component_manager;
    };
}
