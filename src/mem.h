#ifndef DEBBY_MEM_H_
#define DEBBY_MEM_H_

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "./log.h"

namespace debby::mem {

template <typename T>
inline T *allocate(uint64_t elements) {
    auto size{sizeof(T) * elements};
    assert(size > 0);
    void *ptr = malloc(size);
    assert(ptr != NULL);
    log::verbose("allocated %u bytes to address %p", size, ptr);
    return static_cast<T *>(ptr);
}

template <typename T>
inline T *reallocate(T *ptr, uint64_t elements) {
    auto new_size{sizeof(T) * elements};
    assert(new_size > 0);
    void *new_ptr = realloc(ptr, new_size);
    assert(new_ptr != NULL);
    log::verbose("reallocated %u bytes from address %p to address %p", new_size,
                 ptr, new_ptr);
    return static_cast<T *>(new_ptr);
}

template <typename T>
inline void destroy(T *ptr) {
    assert(ptr != NULL);
    log::verbose("freeing address %p", ptr);
    free(ptr);
}
}  // namespace debby::mem

#endif  // DEBBY_MEM_H_
