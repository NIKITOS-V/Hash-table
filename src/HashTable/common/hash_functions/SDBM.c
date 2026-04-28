#include "SDBM.h"

size_t sdbm_hash(const void* obj, size_t len) {
	const uint8_t* str = (const uint8_t*) obj;
	size_t hash = 0;

	for (size_t i = 0; i < len; i++) {
		int c = str[i];

		hash = c + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}
