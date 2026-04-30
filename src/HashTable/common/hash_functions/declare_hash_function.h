#ifndef __DECLARE_HASH_FUNCTION_H__
#define __DECLARE_HASH_FUNCTION_H__

#include <stddef.h>

// Тип указателя на функцию хеширования
typedef size_t (hash_function_t)(const void* obj, size_t len);

#endif /* __DECLARE_HASH_FUNCTION_H__ */
