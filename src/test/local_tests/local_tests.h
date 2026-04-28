#ifndef __LOCAL_TESTS_H__
#define __LOCAL_TESTS_H__

#define NUM_TESTS 7

#define AVL_TREE_SMALL_APPEND_TEST 0
#define AVL_TREE_BID_APPEND_TEST 1
#define AVL_TREE_REMOVE_TEST 2
#define AVL_TREE_FOREACH_TEST 3

#define HT_SMALL_APPEND_TEST 4
#define HT_BIG_APPEND_TEST 5
#define HT_DUPLICATE_TEST 6

void init_tests(void);
int do_test(int test_idx);
int do_all_tests(void);

int AVLTree_small_append_test(void);
int AVLTree_big_append_test(void);
int AVLTree_remove_test(void);
int AVLTree_foreach_test(void);

int HT_small_append_test(void);
int HT_big_append_test(void);
int HT_duplicate_test(void);

#endif // __LOCAL_TESTS_H__
