#include <cstddef>
#include <cstdint>
#include <vector>

#include "gtest/gtest.h"

#include "ecx/containers/sparse_set.hpp"

using ecx::internal::SparseSet;

namespace
{
    template <typename T>
    class SparseSetFixture : public ::testing::Test
    {
    public:
        SparseSet<T> sparse_set;
    };

    using MyTypes = ::testing::Types<uint32_t, uint64_t>;
    TYPED_TEST_SUITE(SparseSetFixture, MyTypes);
}

//
// constructor
//

TEST(SparseSetTest, ConstructorWithoutCustomInitialCapacitySetsDefaultInitialCapacity)
{
    SparseSet<uint32_t> sparse_set_32;
    SparseSet<uint64_t> sparse_set_64;

    EXPECT_EQ(sparse_set_32.initial_capacity(), SparseSet<uint32_t>::DEFAULT_INITIAL_CAPACITY);
    EXPECT_EQ(sparse_set_64.initial_capacity(), SparseSet<uint64_t>::DEFAULT_INITIAL_CAPACITY);
}

TEST(SparseSetTest, ConstructorWithCustomInitialCapacitySetsCustomInitialCapacity)
{
    SparseSet<uint32_t> sparse_set_32(100);
    SparseSet<uint64_t> sparse_set_64(100);

    EXPECT_EQ(sparse_set_32.initial_capacity(), 100);
    EXPECT_EQ(sparse_set_64.initial_capacity(), 100);
}

//
// insert
//

TYPED_TEST(SparseSetFixture, InsertSingleHandleAddsIt)
{
    this->sparse_set.insert(1);

    EXPECT_TRUE(this->sparse_set.contains(1));
    EXPECT_EQ(this->sparse_set.size(), 1);
}

TYPED_TEST(SparseSetFixture, InsertMultipleHandlesAddsAllInInsertionOrder)
{
    this->sparse_set.insert(1);
    this->sparse_set.insert(2);
    this->sparse_set.insert(3);

    EXPECT_TRUE(this->sparse_set.contains(1));
    EXPECT_TRUE(this->sparse_set.contains(2));
    EXPECT_TRUE(this->sparse_set.contains(3));
    EXPECT_EQ(this->sparse_set.size(), 3);

    std::vector<TypeParam> result(this->sparse_set.begin(), this->sparse_set.end());
    EXPECT_EQ(result, (std::vector<TypeParam>{1, 2, 3}));
}

TYPED_TEST(SparseSetFixture, InsertDuplicateHandleAsserts)
{
    this->sparse_set.insert(1);

    EXPECT_DEATH(this->sparse_set.insert(1), "");
}

//
// contains
//

TYPED_TEST(SparseSetFixture, ContainsExistingHandleReturnsTrue)
{
    this->sparse_set.insert(1);

    EXPECT_TRUE(this->sparse_set.contains(1));
}

TYPED_TEST(SparseSetFixture, ContainsNonExistingHandleReturnsFalse)
{
    EXPECT_FALSE(this->sparse_set.contains(1));
}

TYPED_TEST(SparseSetFixture, ContainsErasedHandleReturnsFalse)
{
    this->sparse_set.insert(1);
    this->sparse_set.erase(1);

    EXPECT_FALSE(this->sparse_set.contains(1));
}

//
// erase
//

TYPED_TEST(SparseSetFixture, EraseSingleHandleRemovesIt)
{
    this->sparse_set.insert(1);
    this->sparse_set.erase(1);

    EXPECT_FALSE(this->sparse_set.contains(1));
    EXPECT_EQ(this->sparse_set.size(), 0);
}

TYPED_TEST(SparseSetFixture, EraseLastHandleRemovesIt)
{
    this->sparse_set.insert(1);
    this->sparse_set.insert(2);
    this->sparse_set.erase(2);

    EXPECT_TRUE(this->sparse_set.contains(1));
    EXPECT_FALSE(this->sparse_set.contains(2));
    EXPECT_EQ(this->sparse_set.size(), 1);
}

TYPED_TEST(SparseSetFixture, EraseNonLastHandleCopiesLastIntoRemovedSlotReindexesLastRemovesOriginalLast)
{
    this->sparse_set.insert(1);
    this->sparse_set.insert(2);
    this->sparse_set.insert(3);
    this->sparse_set.erase(1);

    // Proves 1 was removed
    EXPECT_FALSE(this->sparse_set.contains(1));
    EXPECT_TRUE(this->sparse_set.contains(2));
    EXPECT_TRUE(this->sparse_set.contains(3));
    EXPECT_EQ(this->sparse_set.size(), 2);

    // Proves last was copied into 1's slot
    std::vector<TypeParam> result(this->sparse_set.begin(), this->sparse_set.end());
    EXPECT_EQ(result, (std::vector<TypeParam>{3, 2}));

    // Proves last was reindexed
    this->sparse_set.erase(3);
    EXPECT_TRUE(this->sparse_set.contains(2));
    EXPECT_FALSE(this->sparse_set.contains(3));
    EXPECT_EQ(this->sparse_set.size(), 1);
}

TYPED_TEST(SparseSetFixture, EraseNonExistingHandleAsserts)
{
    EXPECT_DEATH(this->sparse_set.erase(1), "");
}

//
// clear
//

TYPED_TEST(SparseSetFixture, ClearEmptyNoOp)
{
    this->sparse_set.clear();

    EXPECT_EQ(this->sparse_set.size(), 0);
}

TYPED_TEST(SparseSetFixture, ClearNonEmptyEmptiesIt)
{
    this->sparse_set.insert(1);
    this->sparse_set.insert(2);
    this->sparse_set.clear();

    EXPECT_FALSE(this->sparse_set.contains(1));
    EXPECT_FALSE(this->sparse_set.contains(2));
    EXPECT_EQ(this->sparse_set.size(), 0);
}

TYPED_TEST(SparseSetFixture, ClearDoesNotShrinkCapacity)
{
    for (std::size_t i = 0; i <= this->sparse_set.DEFAULT_INITIAL_CAPACITY; i++)
    {
        this->sparse_set.insert(i);
    }

    std::size_t capacity_before_clear = this->sparse_set.capacity();
    this->sparse_set.clear();

    EXPECT_EQ(this->sparse_set.capacity(), capacity_before_clear);
}

//
// reset
//

TYPED_TEST(SparseSetFixture, ResetEmptyWithinInitialCapacityNoOp)
{
    this->sparse_set.reset();

    EXPECT_EQ(this->sparse_set.size(), 0);
    EXPECT_EQ(this->sparse_set.capacity(), this->sparse_set.initial_capacity());
}

TYPED_TEST(SparseSetFixture, ResetNonEmptyWithinInitialCapacityEmptiesIt)
{
    this->sparse_set.insert(1);
    this->sparse_set.insert(2);
    this->sparse_set.reset();

    EXPECT_FALSE(this->sparse_set.contains(1));
    EXPECT_FALSE(this->sparse_set.contains(2));
    EXPECT_EQ(this->sparse_set.size(), 0);
    EXPECT_EQ(this->sparse_set.capacity(), this->sparse_set.initial_capacity());
}

TYPED_TEST(SparseSetFixture, ResetEmptyExceedsInitialCapacityShrinksItToInitialCapacity)
{
    for (std::size_t i = 0; i <= this->sparse_set.DEFAULT_INITIAL_CAPACITY; i++)
    {
        this->sparse_set.insert(i);
    }

    EXPECT_GT(this->sparse_set.capacity(), this->sparse_set.DEFAULT_INITIAL_CAPACITY);

    this->sparse_set.clear();
    this->sparse_set.reset();

    EXPECT_EQ(this->sparse_set.capacity(), this->sparse_set.initial_capacity());
}

TYPED_TEST(SparseSetFixture, ResetNonEmptyExceedsInitialCapacityEmptiesItAndShrinksItToInitialCapacity)
{
    for (std::size_t i = 0; i <= this->sparse_set.DEFAULT_INITIAL_CAPACITY; i++)
    {
        this->sparse_set.insert(i);
    }

    EXPECT_GT(this->sparse_set.capacity(), this->sparse_set.DEFAULT_INITIAL_CAPACITY);

    this->sparse_set.reset();

    for (std::size_t i = 0; i <= this->sparse_set.DEFAULT_INITIAL_CAPACITY; i++)
    {
        EXPECT_FALSE(this->sparse_set.contains(i));
    }
    EXPECT_EQ(this->sparse_set.size(), 0);
    EXPECT_EQ(this->sparse_set.capacity(), this->sparse_set.initial_capacity());
}

//
// begin / end
//

TYPED_TEST(SparseSetFixture, BeginEndEmptyBeginEqualsEnd)
{
    EXPECT_EQ(this->sparse_set.begin(), this->sparse_set.end());
}

TYPED_TEST(SparseSetFixture, BeginEndNonEmptyIteratesInsertedHandlesInInsertionOrder)
{
    this->sparse_set.insert(1);
    this->sparse_set.insert(2);
    this->sparse_set.insert(3);

    std::vector<TypeParam> result(this->sparse_set.begin(), this->sparse_set.end());

    EXPECT_EQ(result, (std::vector<TypeParam>{1, 2, 3}));
}
