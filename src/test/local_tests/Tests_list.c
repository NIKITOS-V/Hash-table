#include "local_tests.h"

static const int (*tests[NUM_TESTS])(void);

void init_tests(void) {
	tests[AVL_TREE_SMALL_APPEND_TEST] = AVLTree_small_append_test;
	tests[AVL_TREE_BID_APPEND_TEST] = AVLTree_big_append_test;
	tests[AVL_TREE_REMOVE_TEST] = AVLTree_remove_test;
	tests[AVL_TREE_FOREACH_TEST] = AVLTree_foreach_test;

	tests[HT_SMALL_APPEND_TEST] = HT_small_append_test;
	tests[HT_BIG_APPEND_TEST] = HT_big_append_test;
	tests[HT_DUPLICATE_TEST] = HT_duplicate_test;
}

int do_test(int test_idx) {
    return tests[test_idx]();
}

int do_all_tests(void) {
	for (int i = 0; i < NUM_TESTS; i++) {
		tests[i]();
	}

	return 0;
}
