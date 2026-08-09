#pragma once

#include <utility>
#include <vector>

#include "ecx/containers/sparse_set.hpp"

namespace ecx::internal
{
    template <typename Handle, typename Element>
    class Storage : public SparseSet<Handle>
    {
        using Base = SparseSet<Handle>;

    public:
        using handle_type = Base::handle_type;
        using element_type = Element;
        using const_iterator = typename std::vector<element_type>::const_iterator;

        Storage(std::size_t initial_capacity = Base::DEFAULT_INITIAL_CAPACITY)
            : Base(initial_capacity)
        {
            m_elements.reserve(initial_capacity);
        }

        void insert(handle_type handle, element_type element)
        {
            Base::insert(handle);

            m_elements.push_back(std::move(element));
        }

        void clear()
        {
            Base::clear();

            m_elements.clear();
        }

        void reset()
        {
            Base::reset();

            std::vector<element_type>().swap(m_elements);
            m_elements.reserve(Base::initial_capacity());
        }

        const_iterator begin() const { return m_elements.begin(); }
        const_iterator end() const { return m_elements.end(); }

    protected:
        void on_erase(std::size_t index) override
        {
            m_elements[index] = m_elements.back();
            m_elements.pop_back();
        }

    private:
        std::vector<element_type> m_elements;
    };
}
