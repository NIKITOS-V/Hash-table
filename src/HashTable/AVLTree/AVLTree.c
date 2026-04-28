#include "AVLTree.h"

#include "AVLNode.h"
#include "compare.h"

/* ------ Макросы-константы ------ */

#define MAX_REBALANCE_FACTOR 2

/* ------ Макросы-функции ------ */

#undef max
#define max(a, b) ( (a) > (b) ? (a) : (b) )

/* ------ Приватные поля ------ */

/* ------ Проверки ------ */

static bool is_node_null(AVLNode* node) {
	return node == NULL;
}

/* ------ Геттеры ------ */

static uint64_t get_height(AVLNode* node) {
	return is_node_null(node) ? 0 : node->height;
}

/* ------ Балансировка дерева ------ */

static int64_t calc_balances_factor(AVLNode* subroot) {
	return get_height(subroot->right) - get_height(subroot->left);
}

static void fix_height(AVLNode* subroot) {
	int64_t left_height = get_height(subroot->left);
	int64_t right_height = get_height(subroot->right);

	subroot->height = max(left_height, right_height) + 1;
}

static AVLNode* rotate_left(AVLNode* subroot) {
	AVLNode* right = subroot->right;

	subroot->right = right->left;
	right->left = subroot;

	fix_height(subroot);
	fix_height(right);

	return right;
}

static AVLNode* rotate_right(AVLNode* subroot) {
	AVLNode* left = subroot->left;

	subroot->left = left->right;
	left->right = subroot;

	fix_height(subroot);
	fix_height(left);

	return left;
}

static AVLNode* balance_tree(AVLNode* subroot) {
	fix_height(subroot);

	if (calc_balances_factor(subroot) == MAX_REBALANCE_FACTOR) {
		if (calc_balances_factor(subroot->right) < 0) {
			subroot->right = rotate_right(subroot->right);
		}

		return rotate_left(subroot);
	}

	if (calc_balances_factor(subroot) == -MAX_REBALANCE_FACTOR) {
		if (calc_balances_factor(subroot->left) > 0) {
			subroot->left = rotate_left(subroot->left);
		}

		return rotate_right(subroot);
	}

	return subroot;
}

/* ------ Вставка в дерево ------ */

static int32_t compare_data_blocks(DataBlock* one, DataBlock* two) {
	const void* key_one = one->key;
	size_t key_one_size = one->key_size;

	const void* key_two = two->key;
	size_t key_two_size = two->key_size;

	return compare_objects(key_one, key_one_size, key_two, key_two_size);
}

static AVLNode* insert_into_AVLTree(
	AVLNode* subroot,
	DataBlock* data_block,
	bool* is_success
) {
	if(is_node_null(subroot)) {
		AVLNode* new_node = build_AVLNode(data_block);

		*is_success = (new_node != NULL);

		return new_node;
	}

	if(compare_data_blocks(data_block, &subroot->data_block) == LESS_CMP_CODE) {
		subroot->left = insert_into_AVLTree(subroot->left, data_block, is_success);
	} else {
		subroot->right = insert_into_AVLTree(subroot->right, data_block, is_success);
	}

	return balance_tree(subroot);
}

/* ------ Поиск по дереву ------ */

static AVLNode* search_in_tree_helper(
	AVLNode* subroot,
	const void* key,
	size_t key_size
) {
	if (is_node_null(subroot)) {
		return NULL;
	}

	const void* subroot_key = get_AVLNodes_key(subroot);
	size_t subroot_key_size = get_AVLNodes_key_size(subroot);

	int32_t cmp_result = compare_objects(key, key_size, subroot_key, subroot_key_size);

	if (cmp_result == EQUAL_CMP_CODE) {
		return subroot;
	}

	if (cmp_result == LESS_CMP_CODE) {
		return search_in_tree_helper(subroot->left, key, key_size);
	}

	return search_in_tree_helper(subroot->right, key, key_size);
}

/* ------ Пробежать по элементам AVL дерева ------ */

void AVLTree_foreach_helper(
	AVLNode* node,
	AVLTree_iter_cb* iter_cb,
	void* args
) {
	if (is_node_null(node)) {
		return;
	}

	AVLTree_foreach_helper(node->left, iter_cb, args);

	DataBlock* data_block = &node->data_block;

	iter_cb(
		data_block->key,
		data_block->key_size,
		data_block->value,
		args
	);

	AVLTree_foreach_helper(node->right, iter_cb, args);
}

/* ------ Удаление узлов ------ */

// Поиск AVL узла с минимальным ключом
static AVLNode* find_min_AVLNode(AVLNode* subroot) {
	return is_node_null(subroot->left) ? subroot : find_min_AVLNode(subroot->left);
}

// Исключение AVL узла с минимальным ключом
static AVLNode* remove_min_AVLNode(AVLNode* subroot) {
	if (is_node_null(subroot->left)) {
		return subroot->right;
	}

	subroot->left = remove_min_AVLNode(subroot->left);

	return balance_tree(subroot);
}

// Удаление AVL узла с указанным ключом из AVL дерева
static AVLNode* delete_AVLNode_from_AVLTree(
	AVLNode* subroot,
	const void* key,
	size_t key_size,
	DataBlock_destructor* destructor,
	bool* remove_was
) {
	if (is_node_null(subroot)) {
		*remove_was = false;

		return NULL;
	}

	const void* subroot_key = get_AVLNodes_key(subroot);
	const size_t subroot_key_size = get_AVLNodes_key_size(subroot);

	int32_t cmp_result = compare_objects(key, key_size, subroot_key, subroot_key_size);

	if (cmp_result == LESS_CMP_CODE) {
		subroot->left = delete_AVLNode_from_AVLTree(subroot->left, key, key_size, destructor, remove_was);
	} else if(cmp_result == GREAT_CMP_CODE) {
		subroot->right = delete_AVLNode_from_AVLTree(subroot->right, key, key_size, destructor, remove_was);
	} else {
		AVLNode* left = subroot->left;
		AVLNode* right = subroot->right;

		delete_AVLNode(subroot, destructor);

		if (is_node_null(right)) {
			return left;
		}

		AVLNode* min_node = find_min_AVLNode(right);

		min_node->right = remove_min_AVLNode(right);
		min_node->left = left;

		*remove_was = true;

		return balance_tree(min_node);
	}

	return balance_tree(subroot);
}

// Удаление всех ABL узлов из дерева
static void delete_all_AVLNodes(
	AVLNode* subroot,
	DataBlock_destructor* destructor
) {
	if (is_node_null(subroot)) {
		return;
	}

	delete_all_AVLNodes(subroot->left, destructor);
	delete_all_AVLNodes(subroot->right, destructor);

	delete_AVLNode(subroot, destructor);
}

/* ------ Публичные поля ------ */

void rebuild_AVLTree(AVLTree* tree) {
	tree->root = NULL;

	tree->len = 0;
}

void clear_AVLTree(
	AVLTree* tree,
	DataBlock_destructor* destructor
) {
	delete_all_AVLNodes(tree->root, destructor);

	tree->len = 0;
}

bool append_to_AVLTree(AVLTree* tree, DataBlock* data_block) {
	bool is_success;

	tree->root = insert_into_AVLTree(tree->root, data_block, &is_success);

	if (is_success) {
		tree->len++;
	}

	return is_success;
}

bool delete_from_AVLTree(
	AVLTree* tree,
	const void* key,
	size_t key_size,
	DataBlock_destructor* destructor
) {
	bool remove_was;

	tree->root = delete_AVLNode_from_AVLTree(
		tree->root, key, key_size, destructor, &remove_was
	);

	if (remove_was) {
		tree->len--;
	}

	return remove_was;
}

DataBlock* search_in_AVLtree(
	AVLTree* tree,
	const void* key,
	size_t key_size
) {
	AVLNode* result_node = search_in_tree_helper(tree->root, key, key_size);

	return is_node_null(result_node) ? NULL : &result_node->data_block;
}

void AVLTree_foreach(
	AVLTree* tree,
	AVLTree_iter_cb* iter_cb,
	void* args
) {
	AVLTree_foreach_helper(tree->root, iter_cb, args);
}

size_t get_AVLTree_len(AVLTree* tree) {
	return tree->len;
}

int64_t get_AVLTree_height(AVLTree* tree) {
	return get_height(tree->root);
}
