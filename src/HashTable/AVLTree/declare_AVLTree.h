#ifndef __DECLARE_AVL_TREE_H__
#define __DECLARE_AVL_TREE_H__

#include "declare_DataBlock.h"

typedef struct AVLTree AVLTree;

typedef void (AVLTree_iter_cb) (
    const void* key,
    size_t key_len,
    void* value,
    void* args
);

#endif // __DECLARE_AVL_TREE_H__
