#pragma once

#include <cstddef>
#include <unordered_map>
#include <vector>

#include "ecx/config.hpp"

namespace ecx::internal
{
    template <typename Handle>
    class SparseSet
    {
    public:
        using handle_type = Handle;
        using const_iterator = typename std::vector<handle_type>::const_iterator;

        static constexpr std::size_t DEFAULT_INITIAL_CAPACITY = 32;

        SparseSet(std::size_t initial_capacity = DEFAULT_INITIAL_CAPACITY)
            : m_initial_capacity(initial_capacity)
        {
            m_dense.reserve(m_initial_capacity);
        }

        SparseSet(const SparseSet &) = delete;
        SparseSet &operator=(const SparseSet &) = delete;
        virtual ~SparseSet() = default;

        void insert(handle_type handle)
        {
            ECX_ASSERT(!contains(handle), "SparseSet::insert: handle already exist");

            m_dense.push_back(handle);
            m_sparse[handle] = m_dense.size() - 1;
        }

        bool contains(handle_type handle) const
        {
            return m_sparse.find(handle) != m_sparse.end();
        }

        void erase(handle_type handle)
        {
            ECX_ASSERT(contains(handle), "SparseSet::erase: handle does not exist");

            std::size_t index = m_sparse[handle];
            handle_type last_handle = m_dense.back();

            m_dense[index] = last_handle;
            m_sparse[last_handle] = index;

            on_erase(index);

            m_dense.pop_back();
            m_sparse.erase(handle);
        }

        void clear()
        {
            m_dense.clear();
            m_sparse.clear();
        }

        void reset()
        {
            std::vector<handle_type>().swap(m_dense);
            m_dense.reserve(m_initial_capacity);

            std::unordered_map<handle_type, std::size_t>().swap(m_sparse);
        }

        std::size_t size() const { return m_dense.size(); }
        std::size_t capacity() const { return m_dense.capacity(); }
        std::size_t initial_capacity() const { return m_initial_capacity; }

        const_iterator begin() const { return m_dense.begin(); }
        const_iterator end() const { return m_dense.end(); }

    protected:
        virtual void on_erase(std::size_t index) {}

    private:
        std::size_t m_initial_capacity;

        std::vector<handle_type> m_dense;
        std::unordered_map<handle_type, std::size_t> m_sparse;
    };
}
