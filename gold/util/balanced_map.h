#pragma once

#include <corecrt.h>

template <typename t> t *gold_global_allocate(size_t size = sizeof(t));
template <typename t> void gold_global_deallocate(t *block);

template <typename t, void *(*allocator)(size_t) = gold_global_allocate<void>,
          void (*deallocator)(void *) = gold_global_deallocate<void>>
class gold_balanced_map
{
};