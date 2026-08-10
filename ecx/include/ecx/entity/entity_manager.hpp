#pragma once

#include "ecx/types.hpp"
#include "ecx/containers/sparse_set.hpp"

namespace ecx::internal
{
    class EntityManager
    {
    public:
        EntityManager() = default;
        EntityManager(const EntityManager &) = delete;
        EntityManager &operator=(const EntityManager &) = delete;
        ~EntityManager() = default;

        Entity create();
        bool is_alive(Entity entity) const;

        void destroy(Entity entity);
        void destroy_all();
        void flush();

        void reset();

    private:
        bool m_destroy_all_flag = false;
        bool m_reset_flag = false;

        Entity m_next_entity = 0;

        SparseSet<Entity> m_entities;
        SparseSet<Entity> m_pending_destroy;
    };
}
