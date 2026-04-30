#ifndef __AVL_TREE_H__
#define __AVL_TREE_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "declare_AVLTree.h"
#include "declare_DataBlock.h"

/*
	Структура AVL дерева:
	- root	- корневой узел дерева
	- len	- число элементов в дереве
*/
struct AVLTree {
	struct AVLNode* root;
	size_t len;
};

// Строитель статического AVL дерева
#define to_AVLTree(_root, _len)\
( (AVLTree) {\
	.root = (_root),\
	.len = (_len)\
})

// Пересоздание содержимого AVL дерева
void rebuild_AVLTree(AVLTree* tree);

// Очистка содержимого AVL дерева
void clear_AVLTree(AVLTree* tree, DataBlock_destructor* destructor);

// Копирование и вставка блока данных в AVL дерево
bool append_to_AVLTree(AVLTree* tree, DataBlock* data_block);

// Удаление блока данных по ключу из AVL дерева
bool delete_from_AVLTree(
	AVLTree* tree,
	const void* key,
	size_t key_size,
	DataBlock_destructor* destructor
);

// Поиск блока данных по ключу в AVL дереве
DataBlock* search_in_AVLtree(
	AVLTree* tree,
	const void* key,
	size_t key_size
);

// Пробежать по элементам AVL дерева
void AVLTree_foreach(
	AVLTree* tree,
	AVLTree_iter_cb* iter_cb,
	void* args
);

// Получить количество узлов в AVL дереве
size_t get_AVLTree_len(AVLTree* tree);

// Получить высоту AVL дерева
int64_t get_AVLTree_height(AVLTree* tree);

#endif // __AVL_TREE_H__
