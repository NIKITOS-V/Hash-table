#ifndef __COMPARE_H__
#define __COMPARE_H__

#include <stdint.h>

#define GREAT_CMP_CODE 1
#define EQUAL_CMP_CODE 0
#define LESS_CMP_CODE -1

/*
	Побайтовое сравнение двух объектов
	Возвращает:
		 1 - если первый объект больше второго
		 0 - если объекты равны
		-1 - если второй объект больше первого
*/
int32_t compare_objects(
	const void* obj_1, size_t size_1,
	const void* obj_2, size_t size_2
);

#endif // __COMPARE_H__
