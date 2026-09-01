#pragma once

#include <bit>
#include <cstdint>

namespace ecx::internal
{
    template <typename ValT, typename IdxT, typename GenT, IdxT IdxMask, GenT GenMask>
    struct BaseEntityTraitsConfig
    {
        using value_type = ValT;
        using index_type = IdxT;
        using generation_type = GenT;

        static constexpr index_type INDEX_MASK = IdxMask;
        static constexpr generation_type GENERATION_MASK = GenMask;

        static constexpr std::uint8_t INDEX_BIT_WIDTH = std::popcount(INDEX_MASK);
        static constexpr std::uint8_t GENERATION_BIT_WIDTH = std::popcount(GENERATION_MASK);
    };

    template <typename>
    struct EntityTraitsConfig;

    template <>
    struct EntityTraitsConfig<uint32_t>
        : BaseEntityTraitsConfig<std::uint32_t, std::uint32_t, std::uint16_t, 0xfffff, 0xfff>
    {
    };

    template <>
    struct EntityTraitsConfig<uint64_t>
        : BaseEntityTraitsConfig<std::uint64_t, std::uint64_t, std::uint32_t, 0xffffffffff, 0xffffff>
    {
    };

    template <typename T>
    concept EntityTraitsConfigLike = requires {
        typename T::value_type;
        typename T::index_type;
        typename T::generation_type;

        { T::INDEX_MASK } -> std::convertible_to<typename T::index_type>;
        { T::GENERATION_MASK } -> std::convertible_to<typename T::generation_type>;
    };

    template <EntityTraitsConfigLike TraitsConfig>
    class EntityTraits
    {
        static_assert(TraitsConfig::INDEX_MASK != 0 && TraitsConfig::GENERATION_MASK != 0,
                      "EntityTraits: masks must not be zero");

        static_assert((TraitsConfig::INDEX_MASK & (TraitsConfig::INDEX_MASK + 1)) == 0 &&
                          (TraitsConfig::GENERATION_MASK & (TraitsConfig::GENERATION_MASK + 1)) == 0,
                      "EntityTraits: masks must be contiguous low bits (0xFFF not 0xF0F)");

        static_assert(std::popcount(TraitsConfig::INDEX_MASK) + std::popcount(TraitsConfig::GENERATION_MASK) <= sizeof(typename TraitsConfig::value_type) * 8,
                      "EntityTraits: combined masks bit count exceeds value_type bit capacity");

    public:
        using traits_config = TraitsConfig;

        static constexpr traits_config::value_type pack(traits_config::index_type index, traits_config::generation_type generation)
        {
            return index << traits_config::GENERATION_BIT_WIDTH | (generation & traits_config::GENERATION_MASK);
        }

        static constexpr traits_config::index_type unpack_index(traits_config::value_type handle)
        {
            return handle >> traits_config::GENERATION_BIT_WIDTH;
        }

        static constexpr traits_config::generation_type unpack_generation(traits_config::value_type handle)
        {
            return handle & traits_config::GENERATION_MASK;
        }
    };
}
