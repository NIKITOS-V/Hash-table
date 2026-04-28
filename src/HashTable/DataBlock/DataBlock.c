#include "DataBlock.h"

#include <stdlib.h>

/* ------ Публичные поля ------ */

void clear_DataBlock(DataBlock* data_block) {
	free(data_block->key);

	data_block->key = NULL;
	data_block->value = NULL;

	data_block->key_size = 0;
}

void clear_DataBlock_empty_func(DataBlock* data_block) {
	((void)* data_block);
}
