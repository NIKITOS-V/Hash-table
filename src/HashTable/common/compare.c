#include "compare.h"

int32_t compare_objects(
    const void* obj_1, size_t size_1,
    const void* obj_2, size_t size_2
) {
    if (size_1 > size_2) {
        return GREAT_CMP_CODE;
    }
    if (size_1 < size_2) {
        return LESS_CMP_CODE;
    }

    const unsigned char* ptr_1 = (const unsigned char*) obj_1;
    const unsigned char* ptr_2 = (const unsigned char*) obj_2;

    for (size_t i = 0; i < size_1; i++) {
        if (ptr_1[i] > ptr_2[i]) {
            return GREAT_CMP_CODE;
        }
        if (ptr_1[i] < ptr_2[i]) {
            return LESS_CMP_CODE;
        }
    }

    return EQUAL_CMP_CODE;
}
