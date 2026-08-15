#include "gtest/gtest.h"

#include "ecx/entity/entity_manager.hpp"

using ecx::Entity;
using ecx::internal::EntityManager;

//
// create
//

TEST(EntityManagerTest, CreateMakesEntityAlive)
{
    EntityManager entity_manager;
    Entity entity = entity_manager.create();

    EXPECT_TRUE(entity_manager.is_alive(entity));
}

//
// is_alive
//

TEST(EntityManagerTest, IsAliveReturnsTrueForExistingEntity)
{
    EntityManager entity_manager;
    Entity entity = entity_manager.create();

    EXPECT_TRUE(entity_manager.is_alive(entity));
}

TEST(EntityManagerTest, IsAliveReturnsTrueForEntityMarkedForDestruction)
{
    EntityManager entity_manager;
    Entity entity = entity_manager.create();
    entity_manager.destroy(entity);

    EXPECT_TRUE(entity_manager.is_alive(entity));
}

TEST(EntityManagerTest, IsAliveReturnsFalseForNonExistentEntity)
{
    EntityManager entity_manager;

    EXPECT_FALSE(entity_manager.is_alive(1u));
}

TEST(EntityManagerTest, IsAliveReturnsFalseForFlushedEntity)
{
    EntityManager entity_manager;
    Entity entity = entity_manager.create();
    entity_manager.destroy(entity);
    entity_manager.flush();

    EXPECT_FALSE(entity_manager.is_alive(entity));
}

//
// destroy
//

TEST(EntityManagerTest, DestroyDoesNotTakeEffectUntilFlush)
{
    EntityManager entity_manager;
    Entity entity = entity_manager.create();
    entity_manager.destroy(entity);

    EXPECT_TRUE(entity_manager.is_alive(entity));
}

TEST(EntityManagerTest, DestroyCalledForNonExistentEntityIsNoOp)
{
    EntityManager entity_manager;
    entity_manager.destroy(1u);

    EXPECT_FALSE(entity_manager.is_alive(1u));
}

TEST(EntityManagerTest, DestroyCalledForFlushedEntityIsNoOp)
{
    EntityManager entity_manager;
    Entity entity = entity_manager.create();
    entity_manager.destroy(entity);
    entity_manager.flush();
    entity_manager.destroy(entity);

    EXPECT_FALSE(entity_manager.is_alive(entity));
}

TEST(EntityManagerTest, DestroyCalledMultipleTimesForTheSameEntityIsNoOp)
{
    EntityManager entity_manager;
    Entity entity = entity_manager.create();
    entity_manager.destroy(entity);
    entity_manager.destroy(entity);
    entity_manager.flush();

    EXPECT_FALSE(entity_manager.is_alive(entity));
}

//
// destroy_all
//

TEST(EntityManagerTest, DestroyAllDoesNotTakeEffectUntilFlush)
{
    EntityManager entity_manager;
    Entity entity = entity_manager.create();
    entity_manager.destroy_all();

    EXPECT_TRUE(entity_manager.is_alive(entity));
}

TEST(EntityManagerTest, DestroyAllErasesAllEntitiesAfterFlush)
{
    EntityManager entity_manager;
    Entity entity1 = entity_manager.create();
    Entity entity2 = entity_manager.create();
    entity_manager.destroy_all();
    entity_manager.flush();

    EXPECT_FALSE(entity_manager.is_alive(entity1));
    EXPECT_FALSE(entity_manager.is_alive(entity2));
}

//
// flush
//

TEST(EntityManagerTest, FlushErasesAllEntitiesMarkedForDestruction)
{
    EntityManager entity_manager;
    Entity entity1 = entity_manager.create();
    Entity entity2 = entity_manager.create();
    entity_manager.destroy(entity1);
    entity_manager.destroy(entity2);
    entity_manager.flush();

    EXPECT_FALSE(entity_manager.is_alive(entity1));
    EXPECT_FALSE(entity_manager.is_alive(entity2));
}

TEST(EntityManagerTest, FlushEffectsOnlyEntitiesMarkedForDestruction)
{
    EntityManager entity_manager;
    Entity entity1 = entity_manager.create();
    Entity entity2 = entity_manager.create();
    entity_manager.destroy(entity1);
    entity_manager.flush();

    EXPECT_FALSE(entity_manager.is_alive(entity1));
    EXPECT_TRUE(entity_manager.is_alive(entity2));
}

TEST(EntityManagerTest, FlushCalledWithNoEntitiesMarkedForDestructionIsNoOp)
{
    EntityManager entity_manager;
    Entity entity = entity_manager.create();
    entity_manager.flush();

    EXPECT_TRUE(entity_manager.is_alive(entity));
}

//
// reset
//

TEST(EntityManagerTest, ResetDoesNotTakeEffectUntilFlush)
{
    EntityManager entity_manager;
    Entity entity = entity_manager.create();
    entity_manager.reset();

    EXPECT_TRUE(entity_manager.is_alive(entity));
}

TEST(EntityManagerTest, ResetErasesAllEntitiesAfterFlush)
{
    EntityManager entity_manager;
    Entity entity1 = entity_manager.create();
    Entity entity2 = entity_manager.create();
    entity_manager.reset();
    entity_manager.flush();

    EXPECT_FALSE(entity_manager.is_alive(entity1));
    EXPECT_FALSE(entity_manager.is_alive(entity2));
}
