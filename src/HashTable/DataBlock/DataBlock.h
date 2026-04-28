#ifndef __DATA_BLOCK_H__
#define __DATA_BLOCK_H__

#include <stdbool.h>

#include "declare_DataBlock.h"

/*
	Структура блока данных:
	- key   	- Указатель на ключ
	- key_size	- Размер ключа в байтах
	- value		- Указатель на хранимые данные
*/
struct DataBlock {
	void *key, *value;
	size_t key_size;
};

// Строитель статического блока данных
#define to_DataBlock(_key, _key_size, _value)\
( (DataBlock) {\
	.key = (void*) (_key),\
	.key_size = (size_t) _key_size,\
	.value = (void*) (_value),\
})

#define set_value_in_DataBlock(data_block, _value)\
( (data_block)->value = (void*) _value )

// Очистка содержимого блока данных
void clear_DataBlock(DataBlock* data_block);

void clear_DataBlock_empty_func(DataBlock* data_block);

#endif // __DATA_BLOCK_H__
