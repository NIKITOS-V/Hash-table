#include "HashTable.h"

#include <stdlib.h>
#include <memory.h>

#include "AVLTree.h"
#include "DataBlock.h"
#include "SDBM.h"

/* ------ Макросы-константы ------ */

#define MAX_HT_LOAD_FACTOR 0.75
#define HT_REALLOC_RATIO 2

#define ALLOCATION_FAILURE_CODE -1
#define KEY_ABSENT_CODE -1
#define DUPLICATE_CODE 1
#define OK_CODE 0

/* ------ Макросы-функции ------ */

#define get_forest_ptr(table)\
( &table->forest )

/* ------ Приватные поля ------ */

/* ------ Структура с данными для foreach функции ------ */

typedef struct {
	Array(AVLTree)* new_forest;
	bool except_was;
} ForeachData;

/* ------ Построение хеш-таблицы ------ */

// Пересоздать содержимое массива AVL деревьев
static bool rebuild_forest(Array(AVLTree)* forest, size_t size) {
	if (!rebuild_array(AVLTree)(forest, size)) {
		return false;
	}

	for (size_t i = 0; i < size; i++) {
		rebuild_AVLTree(&get_from_array(forest, i));
	}

	return true;
}

/* ------ Очистка хеш-таблицы ------ */

// Очистка содержимого массива AVL деревьев
static void clear_forest(Array(AVLTree)* forest, DataBlock_destructor* destr) {
	for (size_t i = 0; i < forest->size; i++) {
		clear_AVLTree(&get_from_array(forest, i), destr);
	}

	clear_array(forest);
}

/* ------ Функция хеширования ------ */

// Расчет позиции объекта в массиве по ключу
static size_t calc_obj_pos(
	const void* key,
	size_t key_size,
	size_t hash_border
) {
	return sdbm_hash(key, key_size) % hash_border;
}

/* ------ Вставка в хеш-таблицу ------ */

// Собрать блоки данных из AVL деревьев старого массива
// и добавить в новый массив
static void collect_DataBlocks(
	const void* key,
	size_t key_size,
	void* value,
	ForeachData* f_data
) {
	if (f_data->except_was) {
		return;
	}

	Array(AVLTree)* new_forest = f_data->new_forest;

	DataBlock data_block = to_DataBlock(key, key_size, value);

	size_t obj_pos = calc_obj_pos(key, key_size, get_array_size(new_forest));

	f_data->except_was = !append_to_AVLTree(&get_from_array(new_forest, obj_pos), &data_block);
}

// Увеличить размер массива AVL деревьев путем создания нового массива
// и с последующим его заполнением имеющимися данными
static bool realloc_forest(Array(AVLTree)* forest) {
	Array(AVLTree) new_forest;

	if (!rebuild_forest(&new_forest, get_array_size(forest) * HT_REALLOC_RATIO)) {
		return false;
	}

	ForeachData f_data;

	for (size_t i = 0; i < get_array_size(forest); i++) {
		f_data.except_was = false;
		f_data.new_forest = &new_forest;

		AVLTree* tree = &get_from_array(forest, i);

		AVLTree_foreach(tree, collect_DataBlocks, &f_data);

		if (f_data.except_was) {
			clear_forest(&new_forest, clear_DataBlock_empty_func);

			return false;
		}
	}

	set_array_len(&new_forest, get_array_len(forest));

	clear_forest(forest, clear_DataBlock_empty_func);

	copy_array_data(forest, &new_forest);

	return true;
}

// Копировать объект, исполняющий роль ключа
void* copy_key(const void* key, size_t key_size) {
	void* key_copy = malloc(key_size);

	if (!key_copy) {
		return NULL;
	}

	memcpy(key_copy, key, key_size);

	return key_copy;
}

/* ------ Публичные поля ------ */

ht_t* ht_create(size_t initial_capacity) {
	if (!initial_capacity) {
		return NULL;
	}

	size_t table_size = (size_t) (initial_capacity / MAX_HT_LOAD_FACTOR);

	ht_t* table = (ht_t*)malloc(sizeof(ht_t));

	if (!table) {
		return NULL;
	}

	Array(AVLTree)* forest = get_forest_ptr(table);

	if (!rebuild_forest(forest, table_size)) {
		free(table);
	}

	return table;
}

void ht_destroy(ht_t* table) {
	clear_forest(get_forest_ptr(table), clear_DataBlock);

	free(table);
}

int ht_put(
	ht_t* table,
	const void* key,
	size_t key_size,
	const void* value
) {
	Array(AVLTree)* forest = get_forest_ptr(table);

	size_t obj_pos = calc_obj_pos(key, key_size, get_array_size(forest));

	DataBlock* found_data = search_in_AVLtree(&get_from_array(forest, obj_pos), key, key_size);

	if (found_data) {
		set_value_in_DataBlock(found_data, value);

		return DUPLICATE_CODE;
	}

	if (ht_load_factor(table) >= MAX_HT_LOAD_FACTOR) {
		if ( !realloc_forest(forest) ) {
			return ALLOCATION_FAILURE_CODE;
		}
	}

	void* key_copy = copy_key(key, key_size);

	if (!key_copy) {
		return ALLOCATION_FAILURE_CODE;
	}

	obj_pos = calc_obj_pos(key_copy, key_size, get_array_size(forest));

	DataBlock data_block = to_DataBlock(key_copy, key_size, value);

	if (!append_to_AVLTree(&get_from_array(forest, obj_pos), &data_block)) {
		free(key_copy);

		return ALLOCATION_FAILURE_CODE;
	}

	array_len_incr(forest);

	return OK_CODE;
}

void* ht_get(
	const ht_t* table,
	const void* key,
	size_t key_size
) {
	const Array(AVLTree)* forest = get_forest_ptr(table);

	size_t obj_pos = calc_obj_pos(key, key_size, get_array_size(forest));

	DataBlock* data = search_in_AVLtree(&get_from_array(forest, obj_pos), key, key_size);

	return data ? data->value : NULL;
}

int ht_remove(
	ht_t* table,
	const void* key,
	size_t key_size
) {
	Array(AVLTree)* forest = get_forest_ptr(table);

	size_t obj_pos = calc_obj_pos(key, key_size, get_array_size(forest));

	AVLTree* tree = &get_from_array(forest, obj_pos);

	bool remove_was = delete_from_AVLTree(tree, key, key_size, clear_DataBlock);

	if (!remove_was) {
		return KEY_ABSENT_CODE;
	}

	array_len_decr(forest);

	return OK_CODE;
}

void ht_foreach(ht_t *table, ht_iter_cb cb, void *args) {
	Array(AVLTree)* forest = get_forest_ptr(table);

	for (size_t i = 0; i < get_array_size(forest); i++) {
		AVLTree_foreach(&get_from_array(forest, i), cb, args);
	}
}

size_t ht_size(const ht_t *table) {
	return get_array_len(get_forest_ptr(table));
}

double ht_load_factor(const ht_t* table) {
	return calc_array_load_factor(get_forest_ptr(table));
}
