#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>

/* ------ Макросы замен ------ */

// Структура динамического массива типа T
#define Array(T) Array_##T

// Создать структуру динамического массива типа T
#define build_array(T) build_array_##T

// Пересоздать содержимое структуры динамического массива типа T
#define rebuild_array(T) rebuild_array_##T

// Изменить размеры динамического массива типа T
#define realloc_array(T) realloc_array_##T

// Создать полную копию структуры динамического массива T
#define copy_array(T) copy_array_##T

// Вставить элемент в динамический массив типа T
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

// Краткая запись 'struct Array(T)'
#define StArray(T) struct Array(T)

/* ------ Макросы-проверки ------ */

// Полон ли динамический массив
#define is_array_full(arr)\
( (arr)->len == (arr)->size)

// Корректны ли поля структуры динамического массива для хранения объектов
#define is_array_contains(arr)\
( (arr)->line && (arr)->len <= (arr)->size )

// Пуст ли динамический массив полностью
#define is_array_not_contains(arr)\
( !(arr)->line && !(arr)->len && !(arr)->size )

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

// Вставка в конец динамического массива соответствующего типа
#define append_to_array(arr, value) do {\
	(arr)->line[(arr)->len++] = value;\
} while (0)

// Скопировать данные полей одной структуры динамического массива
// в поля другой структуры динамического массива соответствующего типа
#define copy_array_data(new_arr, arr) do {\
	(new_arr)->line = (arr)->line;\
	(new_arr)->len = (arr)->len;\
	(new_arr)->size = (arr)->size;\
} while (0)

// Рассчитать фактор загруженности динамического массива
#define calc_array_load_factor(arr)\
( (double) (arr)->len / (arr)->size )

/* ------ Строители ------ */

// Создать статическую структуру с полностью пустым динамическим массивом типа T
#define new_Array(T)\
( (StArray(T)) {\
	.line = NULL,\
	.len = 0,\
	.size = 0\
})

// Создать статическую структуру динамического массива типа T на основе переданных данных
#define to_Array(T, _line, _len, _size)\
( (StArray(T)) {\
	.line = (T*) (_line),\
	.len = (_len),\
	.size = (_size)\
})

/* ------ Деструкторы ------ */

// Удалить структуру динамического массива и сам массива
#define delete_array(arr) do {\
	free(arr->line);\
	free(arr);\
	arr = NULL;\
} while (0)

// Очистить динамический массив без удаления его структуры
#define clear_array(arr) do {\
	free((arr)->line);\
	(arr)->line = NULL;\
	(arr)->len = (arr)->size = 0;\
} while (0);

/* ------ Геттеры ------ */

// Получить последний элемент динамического массива
#define get_arrays_back(arr)\
( (arr)->line[(arr)->len - 1] )

// Получить первый элемент динамического массива
#define get_arrays_front(arr)\
( (arr)->line[0] )

// Получить элемент из динамического массива по его индексу
#define get_from_array(arr, idx)\
( (arr)->line[idx] )

// Получить динамический массив из его структуры
#define get_arrays_line(arr)\
( (arr)->line )

// Получить максимальную вместимость динамического массива
#define get_array_size(arr)\
( (arr)->size )

// Получить длину динамического массива
#define get_array_len(arr)\
( (arr)->len )

/* ------ Сеттеры ------ */

// Изменить значение элемента по соответствующему индексу
#define set_in_array(arr, idx, value) do {\
	(arr)->line[idx] = value;\
} while (0)

// Изменить длину динамического массива
#define set_array_len(arr, new_len) do {\
	(arr)->len = new_len;\
} while (0)

/* ------ Инкремент & Декремент ------ */

// Увеличить длину динамического массива на единицу
#define array_len_incr(arr)\
( (arr)->len++ )

// Уменьшить длину динамического массива на единицу
#define array_len_decr(arr)\
( (arr)->len-- )

#endif // __ARRAY_H__
