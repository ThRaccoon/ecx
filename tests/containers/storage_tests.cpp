#include <cstddef>
#include <cstdint>
#include <vector>

#include "gtest/gtest.h"

#include "ecx/containers/storage.hpp"

using ecx::internal::Storage;

namespace
{
    template <typename T>
    class StorageFixture : public ::testing::Test
    {
    public:
        Storage<T, int> storage;
    };

    using MyTypes = ::testing::Types<uint32_t, uint64_t>;
    TYPED_TEST_SUITE(StorageFixture, MyTypes);
}

//
// insert
//

TYPED_TEST(StorageFixture, InsertAddsElementToElements)
{
    this->storage.insert(1, 23);

    std::vector<int> result(this->storage.begin(), this->storage.end());

    EXPECT_EQ(result, (std::vector<int>{23}));
}

//
// clear
//

TYPED_TEST(StorageFixture, ClearEmptiesElements)
{
    this->storage.insert(1, 23);
    this->storage.insert(2, 34);
    this->storage.clear();

    std::vector<int> result(this->storage.begin(), this->storage.end());

    EXPECT_EQ(result, (std::vector<int>{}));
}

//
// reset
//

TYPED_TEST(StorageFixture, ResetEmptiesElements)
{
    this->storage.insert(1, 23);
    this->storage.insert(2, 34);
    this->storage.reset();

    std::vector<int> result(this->storage.begin(), this->storage.end());

    EXPECT_EQ(result, (std::vector<int>{}));
}

//
// on_erase
//

TYPED_TEST(StorageFixture, OnEraseRemovesElementFromElements)
{
    this->storage.insert(1, 23);
    this->storage.erase(1);

    std::vector<int> result(this->storage.begin(), this->storage.end());

    EXPECT_EQ(result, (std::vector<int>{}));
}

TYPED_TEST(StorageFixture, OnEraseNonLastElementCopiesLastIntoItsSlot)
{
    this->storage.insert(1, 23);
    this->storage.insert(2, 34);
    this->storage.insert(3, 45);
    this->storage.erase(1);

    // In elements last (45) now sits where erased (23) was
    std::vector<int> result(this->storage.begin(), this->storage.end());

    EXPECT_EQ(result, (std::vector<int>{45, 34}));
}
