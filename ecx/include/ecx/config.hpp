#pragma once

#ifdef ECX_USE_64BIT_ENTITY
#   define ECX_ENTITY_IS_64BIT 1
#else
#   define ECX_ENTITY_IS_64BIT 0
#endif

#ifdef ECX_ENABLE_WARNINGS
#   include <iostream>
#   define ECX_WARN(msg) (std::cerr << msg << '\n')
#else
#   define ECX_WARN(msg) (void(0))
#endif

#if defined(ECX_DISABLE_ASSERT) || defined(NDEBUG)
#   define ECX_ASSERT(cond, msg) (void(0))
#else
#   include <cassert>
#   define ECX_ASSERT(cond, msg) assert((cond) && (msg))
#endif
