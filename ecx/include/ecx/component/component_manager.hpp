#pragma once

#include <cstddef>
#include <memory>
#include <numeric>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

#include "ecx/config.hpp"
#include "ecx/types.hpp"
#include "ecx/containers/sparse_set.hpp"
#include "ecx/containers/storage.hpp"

namespace ecx::internal
{
    class ComponentManager
    {
    public:
        ComponentManager();
        ComponentManager(const ComponentManager &) = delete;
        ComponentManager &operator=(const ComponentManager &) = delete;
        ~ComponentManager() = default;

        template <typename Component>
        void register_type(std::size_t initial_capacity = SparseSet<Entity>::DEFAULT_INITIAL_CAPACITY)
        {
            std::size_t index = find_storage_index<Component>();

            if (index != INVALID_STORAGE_INDEX)
            {
                ECX_WARN("ComponentManager::register_type: component type already registered");
                return;
            }

            register_storage_index<Component>();
            m_storages.push_back(std::make_unique<Storage<Entity, Component>>(initial_capacity));
        }

        template <typename Component>
        void add(Entity entity, Component component)
        {
            std::size_t index = find_storage_index<Component>();

            if (index == INVALID_STORAGE_INDEX)
            {
                ECX_WARN("ComponentManager::add: component type not registered, auto registering");
                m_storages.push_back(std::make_unique<Storage<Entity, Component>>());
            }

            auto *storage = static_cast<Storage<Entity, Component> *>(m_storages[index].get());

            if (storage->contains(entity))
            {
                ECX_WARN("ComponentManager::add: entity already has this component");
                return;
            }

            storage->insert(entity, std::move(component));
        }

        template <typename Component>
        bool has(Entity entity)
        {
            std::size_t index = find_storage_index<Component>();

            if (index == INVALID_STORAGE_INDEX)
            {
                ECX_WARN("ComponentManager::has: component type not registered");
                return false;
            }

            return m_storages[index]->contains(entity);
        }

        template <typename Component>
        Component *get(Entity entity)
        {
            std::size_t index = find_storage_index<Component>();

            if (index == INVALID_STORAGE_INDEX)
            {
                ECX_WARN("ComponentManager::get: component type not registered");
                return nullptr;
            }

            auto *storage = static_cast<Storage<Entity, Component> *>(m_storages[index].get());

            if (!storage->contains(entity))
            {
                ECX_WARN("ComponentManager::get: entity does not have this component");
                return nullptr;
            }

            return &storage->get(entity);
        }

        template <typename Component>
        void remove(Entity entity)
        {
            std::size_t index = find_storage_index<Component>();

            if (index == INVALID_STORAGE_INDEX)
            {
                ECX_WARN("ComponentManager::remove: component type not registered");
                return;
            }

            if (!m_storages[index]->contains(entity))
            {
                ECX_WARN("ComponentManager::remove: entity does not have this component");
                return;
            }

            m_pending_remove.push_back(std::make_pair(index, entity));
        }

        void remove_all_by_entity(Entity entity);
        void remove_all();
        void flush();

        void reset();

    private:
        static constexpr std::size_t INVALID_STORAGE_INDEX = std::numeric_limits<std::size_t>::max();

        template <typename Component>
        void register_storage_index()
        {
            std::type_index type(typeid(Component));

            ECX_ASSERT(find_storage_index<Component>() == INVALID_STORAGE_INDEX,
                       "ComponentManager::register_storage_index: component type already has storage index");

            m_type_to_index[type] = m_next_index;
            m_next_index++;
        }

        template <typename Component>
        std::size_t find_storage_index() const
        {
            std::type_index type(typeid(Component));

            auto it = m_type_to_index.find(type);
            if (it != m_type_to_index.end())
            {
                return it->second;
            }

            return INVALID_STORAGE_INDEX;
        }

        bool m_remove_all_flag = false;
        bool m_reset_flag = false;

        std::size_t m_next_index = 0;

        std::vector<std::unique_ptr<SparseSet<Entity>>> m_storages;
        std::vector<std::pair<std::size_t, Entity>> m_pending_remove;
        std::unordered_map<std::type_index, std::size_t> m_type_to_index;
    };
}
