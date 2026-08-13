#include <cstddef>
#include <cstdint>
#include <vector>

#include "gtest/gtest.h"

#include "ecx/containers/sparse_set.hpp"

using ecx::internal::SparseSet;

namespace
{
    template <typename T>
    class SparseSetAccessor : public SparseSet<T>
    {
    public:
        using SparseSet<T>::SparseSet;
        using SparseSet<T>::index_of;
    };

    template <typename T>
    class SparseSetFixture : public ::testing::Test
    {
    public:
        SparseSetAccessor<T> sparse_set;
    };

    using MyTypes = ::testing::Types<uint32_t, uint64_t>;
    TYPED_TEST_SUITE(SparseSetFixture, MyTypes);
}

//
// constructor
//

TEST(SparseSetTest, ConstructorDefaultsToInitialCapacityWhenNotProvided)
{
    SparseSet<uint32_t> sparse_set32;
    SparseSet<uint64_t> sparse_set64;

    EXPECT_EQ(sparse_set32.initial_capacity(), SparseSet<uint32_t>::DEFAULT_INITIAL_CAPACITY);
    EXPECT_EQ(sparse_set64.initial_capacity(), SparseSet<uint64_t>::DEFAULT_INITIAL_CAPACITY);
}

TEST(SparseSetTest, ConstructorSetsInitialCapacityWhenProvided)
{
    SparseSet<uint32_t> sparse_set32(64u);
    SparseSet<uint64_t> sparse_set64(128u);

    EXPECT_EQ(sparse_set32.initial_capacity(), 64u);
    EXPECT_EQ(sparse_set64.initial_capacity(), 128u);
}

//
// insert
//

TYPED_TEST(SparseSetFixture, InsertMakesHandleIterable)
{
    this->sparse_set.insert(1u);

    std::vector<TypeParam> result(this->sparse_set.begin(), this->sparse_set.end());

    EXPECT_EQ(result, (std::vector<TypeParam>{1u}));
}

TYPED_TEST(SparseSetFixture, InsertMakesHandleContained)
{
    this->sparse_set.insert(1u);

    EXPECT_TRUE(this->sparse_set.contains(1u));
}

TYPED_TEST(SparseSetFixture, InsertAssertsOnDuplicateHandle)
{
    this->sparse_set.insert(1u);

    EXPECT_DEATH(this->sparse_set.insert(1u), "");
}

//
// contains
//

TYPED_TEST(SparseSetFixture, ContainsReturnsTrueForExistingHandle)
{
    this->sparse_set.insert(1u);

    EXPECT_TRUE(this->sparse_set.contains(1u));
}

TYPED_TEST(SparseSetFixture, ContainsReturnsFalseForNonExistentHandle)
{
    EXPECT_FALSE(this->sparse_set.contains(1u));
}

TYPED_TEST(SparseSetFixture, ContainsReturnsFalseForErasedHandle)
{
    this->sparse_set.insert(1u);
    this->sparse_set.erase(1u);

    EXPECT_FALSE(this->sparse_set.contains(1u));
}

//
// erase
//

TYPED_TEST(SparseSetFixture, EraseMakesHandleNotIterable)
{
    this->sparse_set.insert(1u);
    this->sparse_set.erase(1u);

    std::vector<TypeParam> result(this->sparse_set.begin(), this->sparse_set.end());

    EXPECT_EQ(result, (std::vector<TypeParam>{}));
}

TYPED_TEST(SparseSetFixture, EraseMakesHandleNotContained)
{
    this->sparse_set.insert(1u);
    this->sparse_set.erase(1u);

    EXPECT_FALSE(this->sparse_set.contains(1u));
}

TYPED_TEST(SparseSetFixture, EraseMovesLastHandleIntoErasedIndex)
{
    this->sparse_set.insert(1u);
    this->sparse_set.insert(2u);
    this->sparse_set.insert(3u);
    this->sparse_set.erase(1u);

    std::vector<TypeParam> result(this->sparse_set.begin(), this->sparse_set.end());

    EXPECT_EQ(result, (std::vector<TypeParam>{3u, 2u}));
}

TYPED_TEST(SparseSetFixture, EraseReindexesLastHandle)
{
    this->sparse_set.insert(1u);
    this->sparse_set.insert(2u);
    this->sparse_set.insert(3u);
    this->sparse_set.erase(1u);

    EXPECT_TRUE(this->sparse_set.contains(3u));
}

TYPED_TEST(SparseSetFixture, EraseAssertsOnNonExistentHandle)
{
    EXPECT_DEATH(this->sparse_set.erase(1u), "");
}

TYPED_TEST(SparseSetFixture, EraseAssertsOnErasedHandle)
{
    this->sparse_set.insert(1u);
    this->sparse_set.erase(1u);

    EXPECT_DEATH(this->sparse_set.erase(1u), "");
}

//
// clear
//

TYPED_TEST(SparseSetFixture, ClearMakesHandlesNotIterable)
{
    this->sparse_set.insert(1u);
    this->sparse_set.insert(2u);
    this->sparse_set.clear();

    std::vector<TypeParam> result(this->sparse_set.begin(), this->sparse_set.end());

    EXPECT_EQ(result, (std::vector<TypeParam>{}));
}

TYPED_TEST(SparseSetFixture, ClearMakesHandlesNotContained)
{
    this->sparse_set.insert(1u);
    this->sparse_set.insert(2u);
    this->sparse_set.clear();

    EXPECT_FALSE(this->sparse_set.contains(1u));
    EXPECT_FALSE(this->sparse_set.contains(2u));
}

TYPED_TEST(SparseSetFixture, ClearDoesNotShrinkCapacity)
{
    for (std::size_t i = 0; i <= this->sparse_set.initial_capacity(); i++)
    {
        this->sparse_set.insert(i);
    }

    ASSERT_GT(this->sparse_set.capacity(), this->sparse_set.initial_capacity());

    std::size_t capacity_before_clear = this->sparse_set.capacity();
    this->sparse_set.clear();

    EXPECT_EQ(this->sparse_set.capacity(), capacity_before_clear);
}

//
// reset
//

TYPED_TEST(SparseSetFixture, ResetMakesHandlesNotIterable)
{
    this->sparse_set.insert(1u);
    this->sparse_set.insert(2u);
    this->sparse_set.reset();

    std::vector<TypeParam> result(this->sparse_set.begin(), this->sparse_set.end());

    EXPECT_EQ(result, (std::vector<TypeParam>{}));
}

TYPED_TEST(SparseSetFixture, ResetMakesHandlesNotContained)
{
    this->sparse_set.insert(1u);
    this->sparse_set.insert(2u);
    this->sparse_set.reset();

    EXPECT_FALSE(this->sparse_set.contains(1u));
    EXPECT_FALSE(this->sparse_set.contains(2u));
}

TYPED_TEST(SparseSetFixture, ResetShrinksCapacityToInitialCapacity)
{
    for (std::size_t i = 0; i <= this->sparse_set.initial_capacity(); i++)
    {
        this->sparse_set.insert(i);
    }

    ASSERT_GT(this->sparse_set.capacity(), this->sparse_set.initial_capacity());

    this->sparse_set.reset();

    EXPECT_EQ(this->sparse_set.capacity(), this->sparse_set.initial_capacity());
}

//
// empty
//

TYPED_TEST(SparseSetFixture, EmptyReturnsTrueWhenSizeIsEqualToZero)
{
    EXPECT_TRUE(this->sparse_set.empty());
}

TYPED_TEST(SparseSetFixture, EmptyReturnsFalseWhenSizeIsGreaterThanZero)
{
    this->sparse_set.insert(1u);

    EXPECT_FALSE(this->sparse_set.empty());
}

//
// size
//

TYPED_TEST(SparseSetFixture, SizeInitiallyIsEqualToZero)
{
    EXPECT_EQ(this->sparse_set.size(), 0u);
}

TYPED_TEST(SparseSetFixture, SizeIncrementsWhenHandleIsInserted)
{
    this->sparse_set.insert(1u);

    EXPECT_EQ(this->sparse_set.size(), 1u);
}

TYPED_TEST(SparseSetFixture, SizeDecrementsWhenHandleIsErased)
{
    this->sparse_set.insert(1u);
    this->sparse_set.insert(2u);
    this->sparse_set.erase(1u);

    EXPECT_EQ(this->sparse_set.size(), 1u);
}

//
// capacity
//

TYPED_TEST(SparseSetFixture, CapacityInitiallyIsEqualToInitialCapacity)
{
    EXPECT_EQ(this->sparse_set.capacity(), this->sparse_set.initial_capacity());
}

TYPED_TEST(SparseSetFixture, CapacityIsGreaterThanInitialCapacityAfterResize)
{
    for (std::size_t i = 0; i <= this->sparse_set.initial_capacity(); i++)
    {
        this->sparse_set.insert(i);
    }

    EXPECT_GT(this->sparse_set.capacity(), this->sparse_set.initial_capacity());
}

//
// initial_capacity
//

TYPED_TEST(SparseSetFixture, InitialCapacityInitiallyIsEqualToCapacity)
{
    EXPECT_EQ(this->sparse_set.initial_capacity(), this->sparse_set.capacity());
}

TYPED_TEST(SparseSetFixture, InitialCapacityRemainsUnchangedAfterResize)
{
    std::size_t initial_capacity_before_resize = this->sparse_set.initial_capacity();

    for (std::size_t i = 0; i <= this->sparse_set.initial_capacity(); i++)
    {
        this->sparse_set.insert(i);
    }

    ASSERT_GT(this->sparse_set.capacity(), initial_capacity_before_resize);

    EXPECT_EQ(this->sparse_set.initial_capacity(), initial_capacity_before_resize);
}

//
// begin / end
//

TYPED_TEST(SparseSetFixture, BeginEqualsEndWhenEmpty)
{
    EXPECT_EQ(this->sparse_set.begin(), this->sparse_set.end());
}

TYPED_TEST(SparseSetFixture, BeginIteratesAllHandlesToEndWhenNotEmpty)
{
    this->sparse_set.insert(1u);
    this->sparse_set.insert(2u);

    std::vector<TypeParam> result(this->sparse_set.begin(), this->sparse_set.end());

    EXPECT_EQ(result, (std::vector<TypeParam>{1u, 2u}));
}

//
// index_of
//

TYPED_TEST(SparseSetFixture, IndexOfReturnsIndexOfHandle)
{
    this->sparse_set.insert(42u);
    this->sparse_set.insert(7u);
    this->sparse_set.insert(15u);

    EXPECT_EQ(this->sparse_set.index_of(42u), 0u);
    EXPECT_EQ(this->sparse_set.index_of(7u), 1u);
    EXPECT_EQ(this->sparse_set.index_of(15u), 2u);
}

TYPED_TEST(SparseSetFixture, IndexOfAssertsOnNonExistentHandle)
{
    EXPECT_DEATH(this->sparse_set.index_of(1u), "");
}

TYPED_TEST(SparseSetFixture, IndexOfAssertsOnErasedHandle)
{
    this->sparse_set.insert(1u);
    this->sparse_set.erase(1u);

    EXPECT_DEATH(this->sparse_set.index_of(1u), "");
}
