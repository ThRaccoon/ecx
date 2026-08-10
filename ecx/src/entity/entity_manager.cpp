#include "ecx/entity/entity_manager.hpp"

#include <cstdlib>
#include <iostream>
#include <limits>

namespace ecx::internal
{
    Entity EntityManager::create()
    {
        if (m_next_entity == std::numeric_limits<Entity>::max())
        {
            std::cerr << "EntityManager::create: entity overflow\n";
            std::abort();
        }

        Entity entity = m_next_entity;
        m_next_entity++;

        m_entities.insert(entity);

        return entity;
    }

    bool EntityManager::is_alive(Entity entity) const
    {
        return m_entities.contains(entity);
    }

    void EntityManager::destroy(Entity entity)
    {
        if (!m_entities.contains(entity))
        {
            ECX_WARN("EntityManager::destroy: entity does not exist");
            return;
        }

        if (m_pending_destroy.contains(entity))
        {
            ECX_WARN("EntityManager::destroy: entity already pending destroy");
            return;
        }

        m_pending_destroy.insert(entity);
    }

    void EntityManager::destroy_all()
    {
        m_destroy_all_flag = true;
    }

    void EntityManager::flush()
    {
        if (m_reset_flag)
        {
            m_entities.reset();
            m_pending_destroy.reset();
            m_next_entity = 0;
        }
        else if (m_destroy_all_flag)
        {
            m_entities.clear();
            m_pending_destroy.clear();
        }
        else
        {
            for (Entity e : m_pending_destroy)
            {
                m_entities.erase(e);
            }

            m_pending_destroy.clear();
        }

        m_reset_flag = false;
        m_destroy_all_flag = false;
    }

    void EntityManager::reset()
    {
        m_reset_flag = true;
    }
}
