#pragma once

#include <cstdint>

#include "ecx/config.hpp"

namespace ecx
{
#if ECX_ENTITY_IS_64BIT
    using Entity = uint64_t;
#else
    using Entity = uint32_t;
#endif
}
