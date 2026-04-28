#include "AVLNode.h"

#include <stdlib.h>

#include "DataBlock.h"

/* ------ Публичные поля ------ */

AVLNode* build_AVLNode(DataBlock* data_block) {
	AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));

	if (!node) {
		return NULL;
	}

	node->data_block = *data_block;

	node->left = NULL;
	node->right = NULL;

	node->height = AVL_NODE_INIT_HEIGHT;

	return node;
}

void delete_AVLNode(AVLNode* node, DataBlock_destructor* destructor) {
	destructor(&node->data_block);

	free(node);
}
