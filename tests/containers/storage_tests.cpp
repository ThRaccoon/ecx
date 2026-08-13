#include <cstddef>
#include <cstdint>
#include <vector>

#include "gtest/gtest.h"

#include "ecx/containers/storage.hpp"

using ecx::internal::Storage;

namespace
{
    template <typename T>
    class StorageAccessor : public Storage<T, int>
    {
    public:
        using Storage<T, int>::on_erase;
    };

    template <typename T>
    class StorageFixture : public ::testing::Test
    {
    public:
        StorageAccessor<T> storage;
    };

    using MyTypes = ::testing::Types<uint32_t, uint64_t>;
    TYPED_TEST_SUITE(StorageFixture, MyTypes);
}

//
// insert
//

TYPED_TEST(StorageFixture, InsertMakesElementIterable)
{
    this->storage.insert(1u, 23);

    std::vector<int> result(this->storage.begin(), this->storage.end());

    EXPECT_EQ(result, (std::vector<int>{23}));
}

//
// get
//

TYPED_TEST(StorageFixture, GetReturnsElement)
{
    this->storage.insert(1u, 23);

    EXPECT_EQ(this->storage.get(1u), 23);
}

TYPED_TEST(StorageFixture, GetAssertsOnNonExistentElement)
{
    EXPECT_DEATH(this->storage.get(1u), "");
}

TYPED_TEST(StorageFixture, GetAssertsOnErasedElement)
{
    this->storage.insert(1u, 23);
    this->storage.erase(1u);

    EXPECT_DEATH(this->storage.get(1u), "");
}

//
// clear
//

TYPED_TEST(StorageFixture, ClearMakesElementsNotIterable)
{
    this->storage.insert(1u, 23);
    this->storage.insert(2u, 34);
    this->storage.clear();

    std::vector<int> result(this->storage.begin(), this->storage.end());

    EXPECT_EQ(result, (std::vector<int>{}));
}

//
// reset
//

TYPED_TEST(StorageFixture, ResetMakesElementsNotIterable)
{
    this->storage.insert(1u, 23);
    this->storage.insert(2u, 34);
    this->storage.reset();

    std::vector<int> result(this->storage.begin(), this->storage.end());

    EXPECT_EQ(result, (std::vector<int>{}));
}

//
// begin / end
//

TYPED_TEST(StorageFixture, BeginEqualsEndWhenEmpty)
{
    EXPECT_EQ(this->storage.begin(), this->storage.end());
}

TYPED_TEST(StorageFixture, BeginIteratesAllElementsToEndWhenNotEmpty)
{
    this->storage.insert(1u, 23);
    this->storage.insert(2u, 34);

    std::vector<int> result(this->storage.begin(), this->storage.end());

    EXPECT_EQ(result, (std::vector<int>{23, 34}));
}

//
// on_erase
//

TYPED_TEST(StorageFixture, OnEraseMakesElementNotIterable)
{
    this->storage.insert(1u, 23);
    this->storage.erase(1u);

    std::vector<int> result(this->storage.begin(), this->storage.end());

    EXPECT_EQ(result, (std::vector<int>{}));
}

TYPED_TEST(StorageFixture, OnEraseMovesLastElementIntoErasedIndex)
{
    this->storage.insert(1u, 23);
    this->storage.insert(2u, 34);
    this->storage.insert(3u, 45);
    this->storage.erase(1u);

    std::vector<int> result(this->storage.begin(), this->storage.end());

    EXPECT_EQ(result, (std::vector<int>{45, 34}));
}
