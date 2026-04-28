#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>

// ------ Макросы замен ------

#define Array(T) Array_##T
#define build_array(T) build_array_##T
#define rebuild_array(T) rebuild_array_##T
#define realloc_array(T) realloc_array_##T
#define copy_array(T) copy_array_##T
#define insert_into_array(T) insert_into_array_##T

/* ------ Макрос структуры ------ */

/*
	Структура динамического массива:
	- line	- Массив с элементами типа T
	- len 	- Число элементов в массиве
	- size	- Максимальная вместимость массива
*/
#define declare_array(T)\
struct Array(T) {\
	T* line;\
	size_t len;\
	size_t size;\
}

#define StArray(T) struct Array(T)

/* ------ Макросы-проверки ------ */

#define is_array_full(arr)\
( (arr)->len == (arr)->size)

#define is_array_contains(arr)\
( (arr)->line && (arr)->len <= (arr)->size )

#define is_array_not_contains(arr)\
( !(arr)->line && !(arr)->len && !(arr)->size )

#define is_array_exist(arr)\
( (arr) && is_array_contains(arr) )

#define is_array_not_exist(arr)\
( (arr) && is_array_not_contains(arr) )

#define is_array_correct(arr)\
( (arr) && (is_array_contains(arr) || is_array_not_contains(arr)) )

/* ------ Макросы объявлений функций ------ */

#define declare_build_array(T)\
StArray(T)* build_array(T) (size_t size)

#define declare_rebuild_array(T)\
bool rebuild_array(T) (StArray(T)* arr, size_t size)

#define declare_realloc_array(T)\
bool realloc_array(T) (StArray(T)* arr, size_t new_size)

#define declare_copy_array(T)\
StArray(T)* copy_array(T) (StArray(T)* arr)

#define declare_insert_into_array(T)\
bool insert_into_array(T) (StArray(T)* arr, size_t index, T value)

/* ------ Макросы функций ------ */

#define generate_rebuild_array(T)\
declare_rebuild_array(T) {\
	if (!size) {\
		arr->line = NULL;\
	} else {\
		arr->line = (T*)calloc(size, sizeof(T));\
		if (!arr->line) {\
			return false;\
		}\
	}\
	arr->size = size;\
	arr->len = 0;\
	return true;\
}

#define generate_build_array(T)\
declare_build_array(T) { \
	StArray(T)* arr = (StArray(T)*)malloc(sizeof(StArray(T)));\
	if (!arr) return NULL;\
	if (!size) {\
		arr->line = NULL;\
	} else {\
		arr->line = (T*)calloc(size, sizeof(T));\
		if (!arr->line) {\
			free(arr);\
			return NULL;\
		}\
	}\
	arr->size = size;\
	arr->len = 0;\
	return arr;\
}

#define generate_realloc_array(T)\
declare_realloc_array(T) {\
	if (!arr->line) {\
		arr->line = (T*)calloc(new_size, sizeof(T));\
		if (!arr->line) return false;\
	} else {\
		T* new_line = (T*)realloc(arr->line, new_size * sizeof(T));\
		if (!new_line) return false;\
		arr->line = new_line;\
	}\
	arr->size = new_size;\
	if (arr->size < arr->len) {\
		arr->len = arr->size;\
	}\
	return true;\
}

#define generate_copy_array(T)\
declare_copy_array(T) {\
	StArray(T)* copy = build_array(T)(arr->len);\
	if (!copy) return NULL;\
	for (copy->len = 0; copy->len < arr->len; copy->len++) {\
		copy->line[copy->len] = arr->line[copy->len];\
	}\
	return copy;\
}

#define generate_insert_into_array(T)\
declare_insert_into_array(T) {\
	memmove(\
		arr->line + index + 1,\
		arr->line + index,\
		(arr->len - index) * sizeof(T)\
	);\
	arr->line[index] = value;\
	arr->len++;\
	return true;\
}

/* ------ Макросы-функции ------ */

#define append_to_array(arr, value) do {\
	(arr)->line[(arr)->len++] = value;\
} while (0)

#define copy_array_data(new_arr, arr) do {\
	(new_arr)->line = (arr)->line;\
	(new_arr)->len = (arr)->len;\
	(new_arr)->size = (arr)->size;\
} while (0)

#define calc_array_load_factor(arr)\
( (double) (arr)->len / (arr)->size )

/* ------ Строители ------ */

#define new_Array(T)\
( (StArray(T)) {\
	.line = NULL,\
	.len = 0,\
	.size = 0\
})

#define to_Array(T, _line, _len, _size)\
( (StArray(T)) {\
	.line = (T*) (_line),\
	.len = (_len),\
	.size = (_size)\
})

/* ------ Деструкторы ------ */

#define delete_array(arr) do {\
	free(arr->line);\
	free(arr);\
	arr = NULL;\
} while (0)

#define clear_array(arr) do {\
	free((arr)->line);\
	(arr)->line = NULL;\
	(arr)->len = (arr)->size = 0;\
} while (0);

/* ------ Геттеры ------ */

#define get_arrays_back(arr)\
( (arr)->line[(arr)->len - 1] )

#define get_arrays_front(arr)\
( (arr)->line[0] )

#define get_from_array(arr, idx)\
( (arr)->line[idx] )

#define get_arrays_line(arr)\
( (arr)->line )

#define get_array_size(arr)\
( (arr)->size )

#define get_array_len(arr)\
( (arr)->len )

/* ------ Сеттеры ------ */

#define set_in_array(arr, idx, value) do {\
	(arr)->line[idx] = value;\
} while (0)

#define set_array_len(arr, new_len) do {\
	(arr)->len = new_len;\
} while (0)

/* ------ Инкремент & Декремент ------ */

#define array_len_incr(arr)\
( (arr)->len++ )

#define array_len_decr(arr)\
( (arr)->len-- )

#endif // __ARRAY_H__
