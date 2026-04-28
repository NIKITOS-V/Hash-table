#include "local_tests.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <memory.h>

#include "AVLTree.h"
#include "DataBlock.h"

int AVLTree_small_append_test(void) {
	printf("\nStart: AVLTree_small_append_test\n");

	printf("\nNumber data: %d\n", 4);

	printf("\nCreate data... ");

	char key1[] = "key1";
	char key2[] = "key2";
	char key3[] = "key3";
	char key4[] = "key4";

	char* key1_ptr = malloc(sizeof(key1));
	char* key2_ptr = malloc(sizeof(key2));
	char* key3_ptr = malloc(sizeof(key3));
	char* key4_ptr = malloc(sizeof(key4));

	memcpy(key1_ptr, key1, sizeof(key1));
	memcpy(key2_ptr, key2, sizeof(key2));
	memcpy(key3_ptr, key3, sizeof(key3));
	memcpy(key4_ptr, key4, sizeof(key4));

	char value1[] = "value1";
	char value2[] = "value2";
	char value3[] = "value3";
	char value4[] = "value4";

	DataBlock data_block1 = to_DataBlock(key1_ptr, sizeof(key1), &value1);
	DataBlock data_block2 = to_DataBlock(key2_ptr, sizeof(key2), &value2);
	DataBlock data_block3 = to_DataBlock(key3_ptr, sizeof(key3), &value3);
	DataBlock data_block4 = to_DataBlock(key4_ptr, sizeof(key4), &value4);

	printf("OK\n");
	printf("Create tree... ");

	AVLTree tree;

	rebuild_AVLTree(&tree);

	printf("OK\n");

	printf("\nAppend data... ");

	if (!append_to_AVLTree(&tree, &data_block1) ||
		!append_to_AVLTree(&tree, &data_block4) ||
		!append_to_AVLTree(&tree, &data_block2) ||
		!append_to_AVLTree(&tree, &data_block3)
	) {
		printf("FAILED\n");

		printf("\nClear tree & memory... ");

		clear_AVLTree(&tree, clear_DataBlock);

		printf("OK\n");
		printf("\nEND OF TEST\n");

		return 0;
	}

	printf("OK\n");

	printf("\nStart search... ");

	DataBlock* found1 = search_in_AVLtree(&tree, key1_ptr, sizeof(key1));
	DataBlock* found2 = search_in_AVLtree(&tree, key2_ptr, sizeof(key2));
	DataBlock* found3 = search_in_AVLtree(&tree, key3_ptr, sizeof(key3));
	DataBlock* found4 = search_in_AVLtree(&tree, key4_ptr, sizeof(key4));

	printf("END\n");
	printf("\n");

	if (found1) {
		printf("Found: %s\n", (char*) (found1->value));
	} else {
		printf("Not found: %s\n", key1);
	}

	if (found2) {
		printf("Found: %s\n", (char*)found2->value);
	} else {
		printf("Not found: %s\n", key2);
	}

	if (found3) {
		printf("Found: %s\n", (char*)found3->value);
	} else {
		printf("Not found: %s\n", key3);
	}

	if (found4) {
		printf("Found: %s\n", (char*)found4->value);
	} else {
		printf("Not found: %s\n", key4);
	}

	printf("\nTree len: %zu\n", get_AVLTree_len(&tree));
	printf("Tree height: %" PRId64 "\n", get_AVLTree_height(&tree));

	printf("\nClear tree... ");

	clear_AVLTree(&tree, clear_DataBlock);

	printf("OK\n");

	printf("\nEND OF TEST\n");

	return 0;
}

#define VALUES_COUNT 100

int AVLTree_big_append_test(void) {
	printf("\nStart: AVLTree_big_append_test\n");

	printf("\nNumber data: %d\n", VALUES_COUNT);

	printf("\nCreate data... ");

	int* keys[VALUES_COUNT];
	int values[VALUES_COUNT];
	DataBlock data[VALUES_COUNT];

	for (int i = 0, q = 0; i < VALUES_COUNT; i++, q = !q) {
		keys[i] = malloc(sizeof(int*));
		*keys[i] = i;

		values[i] = i;

		data[i] = to_DataBlock(keys[i], sizeof(keys[0]), &values[i]);
	}

	printf("OK\n");
	printf("Create tree... ");

	AVLTree tree;

	rebuild_AVLTree(&tree);

	printf("OK\n");

	printf("\nAppend data... ");

	for (int i = 0; i < VALUES_COUNT; i++) {
		if (!append_to_AVLTree(&tree, &data[i])) {
			printf("FAILED\n");

			printf("\nClear tree & memory... ");

			clear_AVLTree(&tree, clear_DataBlock);

			printf("OK\n");
			printf("\nEND OF TEST\n");

			return 0;
		}
	}

	printf("OK\n");

	printf("\nStart search...\n");
	printf("\n");

	int fail_count = 0;

	for (int i = 0; i < VALUES_COUNT; i++) {
		DataBlock* found = search_in_AVLtree(&tree, keys[i], sizeof(keys[0]));

		if (found) {
			printf("Found: %d, %d\n", *((int*) found->key), *((int*) found->value) );
		} else {
			printf("Not found: %d\n", *keys[i]);
			fail_count++;
		}
	}

	printf("\nEND Search\n");

	printf("\nWas found: %d\n", VALUES_COUNT - fail_count);
	printf("Was NOT found: %d\n", fail_count);

	printf("\nTree len: %zu\n", get_AVLTree_len(&tree));
	printf("Tree height: %" PRId64 "\n", get_AVLTree_height(&tree));

	printf("\nClear tree & memory... ");

	clear_AVLTree(&tree, clear_DataBlock);

	printf("OK\n");

	printf("\nEND OF TEST\n");

	return 0;
}

#undef VALUES_COUNT

#define VALUES_COUNT 100

int AVLTree_remove_test(void) {
	printf("\nStart: AVLTree_remove_test\n");

	printf("\nNumber data: %d\n", VALUES_COUNT);

	printf("\nCreate data... ");

	int* keys[VALUES_COUNT];
	int values[VALUES_COUNT];
	DataBlock datas[VALUES_COUNT];

	for (int i = 0, q = 0; i < VALUES_COUNT; i++, q = !q) {
		keys[i] = malloc(sizeof(int*));
		*keys[i] = i;

		values[i] = i;

		datas[i] = to_DataBlock(keys[i], sizeof(keys[0]), &values[i]);
	}

	printf("OK\n");
	printf("Create AVL tree... ");

	AVLTree tree;

	rebuild_AVLTree(&tree);

	printf("OK\n");

	printf("\nAppend data... ");

	for (int i = 0; i < VALUES_COUNT; i++) {
		if (!append_to_AVLTree(&tree, &datas[i])) {
			printf("FAILED\n");

			printf("\nClear tree & memory... ");

			clear_AVLTree(&tree, clear_DataBlock);

			printf("OK\n");
			printf("\nEND OF TEST\n");

			return 0;
		}
	}

	printf("OK\n");

	printf("\nTree len: %zu\n", get_AVLTree_len(&tree));
	printf("Tree height: %" PRId64 "\n", get_AVLTree_height(&tree));

	printf("\nStart remove and search...\n");
	printf("\n");

	int search_fail = 0;
	int remove_fail = 0;

	for (int i = 0; i < VALUES_COUNT; i++) {
		for (int j = i; j < VALUES_COUNT; j++) {
			DataBlock* found = search_in_AVLtree(&tree, keys[j], sizeof(keys[0]));

			if (!found) {
				printf("Not found: %d\n", *keys[j]);
				search_fail++;
			}
		}

		if (!delete_from_AVLTree(&tree, keys[i], sizeof(keys[0]), clear_DataBlock)) {
			printf("Not remove: %d\n", *keys[i]);
			remove_fail++;
		}
	}

	printf("\nEND remove and search\n");

	printf("\nRemove fail: %d\n", remove_fail);
	printf("Search fail: %d\n", search_fail);

	printf("\nTry search again... \n");

	for (int i = 0; i < VALUES_COUNT; i++) {
		for (int j = i; j < VALUES_COUNT; j++) {
			DataBlock* found = search_in_AVLtree(&tree, keys[j], sizeof(keys[0]));

			if (found) {
				search_fail++;
			}
		}
	}

	printf("\nEND search\n");

	printf("\nSearch fail: %d\n", search_fail);

	printf("\nTree len: %zu\n", get_AVLTree_len(&tree));
	printf("Tree height: %" PRId64 "\n", get_AVLTree_height(&tree));

	printf("\nClear tree & memory... ");

	clear_AVLTree(&tree, clear_DataBlock);

	printf("OK\n");
	printf("\nEND OF TEST\n");

	return 0;
}

#undef VALUES_COUNT

#define VALUES_COUNT 100

void print_datas(const void *key, size_t key_len, void *value, int* count) {
	((void*) key_len);

	printf(
		"%d: (%d, %d)\n",
		*count,
		*((int*) key),
		*((int*) value)
	);

	*count += 1;
}

int AVLTree_foreach_test(void) {
	printf("\nStart: AVLTree_remove_test\n");

	printf("\nNumber data: %d\n", VALUES_COUNT);

	printf("\nCreate data... ");

	int* keys[VALUES_COUNT];
	int values[VALUES_COUNT];
	DataBlock datas[VALUES_COUNT];

	for (int i = 0, q = 0; i < VALUES_COUNT; i++, q = !q) {
		keys[i] = malloc(sizeof(int*));
		*keys[i] = i;

		values[i] = i;

		datas[i] = to_DataBlock(keys[i], sizeof(keys[0]), &values[i]);
	}

	printf("OK\n");
	printf("Create AVL tree... ");

	AVLTree tree;

	rebuild_AVLTree(&tree);

	printf("OK\n");

	printf("\nAppend data... ");

	for (int i = 0; i < VALUES_COUNT; i++) {
		if (!append_to_AVLTree(&tree, &datas[i])) {
			printf("FAILED\n");

			printf("\nClear tree & memory... ");

			clear_AVLTree(&tree, clear_DataBlock);

			printf("OK\n");
			printf("\nEND OF TEST\n");

			return 0;
		}
	}

	printf("OK\n");

	printf("\nTree len: %" PRId64 "\n", get_AVLTree_len(&tree));
	printf("Tree height: %" PRId64 "\n", get_AVLTree_height(&tree));

	printf("\nStart foreach...\n");
	printf("\n");

	int count = 0;

	AVLTree_foreach(&tree, print_datas, &count);

	printf("\nEND foreach\n");

	printf("\nCount: %d\n", count);

	printf("\nTree len: %zu\n", get_AVLTree_len(&tree));
	printf("Tree height: %" PRId64 "\n", get_AVLTree_height(&tree));

	printf("\nClear tree & memory... ");

	clear_AVLTree(&tree, clear_DataBlock);

	printf("OK\n");
	printf("\nEND OF TEST\n");

	return 0;
}

#undef VALUES_COUNT
