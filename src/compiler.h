#ifndef SHMB_COMPILER_H
#define SHMB_COMPILER_H

#include <stdint.h>
#include <stddef.h>

#define __unused __attribute__((unused))

#define container_of(p, s, m) ((s *)((uintptr_t)(p) - offsetof(s, m)))

#endif
