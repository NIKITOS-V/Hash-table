#ifndef __AVL_NODE_H__
#define __AVL_NODE_H__

#include <stdint.h>

#include "declare_AVLNode.h"
#include "DataBlock.h"

#define AVL_NODE_INIT_HEIGHT 1

/*
	Структура AVL узла:
	- data_block	- Блок данных, хранящий ключ и значение
	- left_idx		- Указатель левого потомка
	- right_idx		- Указатель правого потомка
	- height		- Высота поддерева
*/
struct AVLNode {
	DataBlock data_block;
	AVLNode *left, *right;
	int64_t height;
};

// Строитель статического AVL узла
#define to_AVLNode(_data_block, _left_idx, _right_idx, _height)\
( (AVLNode) {\
	.data_block = (_data_block),\
	.left_idx = (size_t) (_left_idx),\
	.right_idx = (size_t) (_right_idx),\
	.height = (int64_t) (_height)\
})

// Создание AVL узла
AVLNode* build_AVLNode(DataBlock* data_block);

// Удаление AVL узла
void delete_AVLNode(AVLNode* node, DataBlock_destructor* destructor);

#define get_AVLNodes_key(node)\
( node->data_block.key )

#define get_AVLNodes_key_size(node)\
( node->data_block.key_size )

#endif // __AVL_NODE_H__
