#ifndef __SDBM_H__
#define __SDBM_H__

#include "declare_hash_function.h"

// SDBM хеш-функция
size_t sdbm_hash(const void* obj, size_t len);

#endif /* __SDBM_H__ */
