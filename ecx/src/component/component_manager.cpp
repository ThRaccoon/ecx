#include "ecx/component/component_manager.hpp"

namespace ecx::internal
{
    ComponentManager::ComponentManager()
    {
        m_storages.reserve(16);
        m_pending_remove.reserve(32);
    }

    void ComponentManager::remove_all_by_entity(Entity entity)
    {
        for (const auto &storage : m_storages)
        {
            if (storage->contains(entity))
            {
                storage->erase(entity);
            }
        }
    }

    void ComponentManager::remove_all()
    {
        m_remove_all_flag = true;
    }

    void ComponentManager::flush()
    {
        if (m_reset_flag)
        {
            m_next_index = 0;

            std::vector<std::unique_ptr<SparseSet<Entity>>>().swap(m_storages);
            std::vector<std::pair<std::size_t, Entity>>().swap(m_pending_remove);
            std::unordered_map<std::type_index, std::size_t>().swap(m_type_to_index);
        }
        else if (m_remove_all_flag)
        {
            for (const auto &storage : m_storages)
            {
                storage->clear();
            }

            m_pending_remove.clear();
        }
        else
        {
            for (const auto &[index, entity] : m_pending_remove)
            {
                m_storages[index]->erase(entity);
            }

            m_pending_remove.clear();
        }

        m_reset_flag = false;
        m_remove_all_flag = false;
    }

    void ComponentManager::reset()
    {
        m_reset_flag = true;
    }
}
