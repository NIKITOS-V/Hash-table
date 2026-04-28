#include "local_tests.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#include "hastable_iface.h"

#define str(x) #x

#define END_OF_TEST "\nEND OF TEST\n"

static void delete_ht(ht_t* table) {
	printf("\nDestroy table... ");

	ht_destroy(table);

	printf("OK\n");
}

int HT_small_append_test(void) {
	printf("\nStart: HT_small_append_test\n");

	int initial_capacity = 5;
	int values_count = 4;

	printf("\nInitial capacity: %d\n", initial_capacity);
	printf("Values count: %d\n", values_count);

	printf("\nCreate data... ");

	char key1 = 'k';
	int key2 = 67;
	float key3 = 52.52f;
	short key4 = -228;

	char value1[] = "key1_value";
	char value2[] = "key2_value";
	char value3[] = "key3_value";
	char value4[] = "key4_value";

	printf("OK\n");

	printf("Create table... ");

	ht_t* table = ht_create(initial_capacity);

	if (!table) {
		printf("FAIL\n");

		printf("\nEND OF TEST\n");

		return 0;
	}

	printf("OK\n");

	printf("\nAppend data...\n");

	int code;

	code = ht_put(table, &key1, sizeof(key1), &value1);

	printf("\nCode: %d\n", code);

	if (code != 0) {
		printf("\nFAIL\n");

		delete_ht(table);

		printf("\nEND OF TEST\n");

		return 0;
	}

	code = ht_put(table, &key2, sizeof(key2), &value2);

	printf("Code: %d\n", code);

	if (code != 0) {
		printf("\nFAIL\n");

		delete_ht(table);

		printf("\nEND OF TEST\n");

		return 0;
	}

	code = ht_put(table, &key3, sizeof(key3), &value3);

	printf("Code: %d\n", code);

	if (code != 0) {
		printf("\nFAIL\n");

		delete_ht(table);

		printf("\nEND OF TEST\n");

		return 0;
	}

	code = ht_put(table, &key4, sizeof(key4), &value4);

	printf("Code: %d\n", code);

	if (code != 0) {
		printf("\nFAIL\n");

		delete_ht(table);

		printf("\nEND OF TEST\n");

		return 0;
	}

	printf("\nEND append data\n");

	printf("\nStart get data...\n");

	void* f = ht_get(table, &key1, sizeof(key1));

	if (!f) {
		printf("\nFAIL\n");

		delete_ht(table);

		printf("\nEND OF TEST\n");

		return 0;
	}

	printf("\nValue: %s\n", (char*) f);

	f = ht_get(table, &key2, sizeof(key2));

	if (!f) {
		printf("\nFAIL\n");

		delete_ht(table);

		printf("\nEND OF TEST\n");

		return 0;
	}

	printf("Value: %s\n", (char*) f);

	f = ht_get(table, &key3, sizeof(key3));

	if (!f) {
		printf("\nFAIL\n");

		delete_ht(table);

		printf("\nEND OF TEST\n");

		return 0;
	}

	printf("Value: %s\n", (char*) f);

	f = ht_get(table, &key4, sizeof(key4));

	if (!f) {
		printf("\nFAIL\n");

		delete_ht(table);

		printf("\nEND OF TEST\n");

		return 0;
	}

	printf("Value: %s\n", (char*) f);

	printf("\nEND of get\n");

	printf("\nTable len: %llu\n", ht_size(table));
	printf("Load factor: %f\n", ht_load_factor(table));

	ht_destroy(table);

	printf("\nEND OF TEST\n");

	return 0;
}

#define VALUES_COUNT 100

int HT_big_append_test(void) {
	printf("\nStart: " str(HT_big_append_test) "\n");

	int initial_capacity = 5;

	printf("\nInitial capacity: %d\n", initial_capacity);
	printf("Values count: %d\n", VALUES_COUNT);

	printf("\nCreate table... ");

	ht_t* table = ht_create(initial_capacity);

	if (!table) {
		printf("FAIL\n");

		printf(END_OF_TEST);

		return 0;
	}

	printf("OK\n");

	printf("Create values... ");

	int values[VALUES_COUNT];

	for (int i = 0; i < VALUES_COUNT; i++) {
		values[i] = i;
	}

	printf("OK\n");

	{ // TEST BLOCK 1: Create, append and search values
		printf("Create keys... ");

		int keys[VALUES_COUNT];

		for (int i = 0; i < VALUES_COUNT; i++) {
				keys[i] = i;
		}

		printf("OK\n");

		printf("\nAppend data... ");

		for (int i = 0; i < VALUES_COUNT; i++) {
			int code;

			code = ht_put(table, &keys[i], sizeof(keys[0]), &values[i]);

			if (code != 0) {
				printf("\n\nFAIL on iteration: %d\n", i);
				printf("Code: %d\n", code);

				delete_ht(table);

				printf(END_OF_TEST);

				return 0;
			}
		}

		printf("OK\n");

		printf("\nHt len: %llu\n", ht_size(table));
		printf("Ht load factor: %f\n", ht_load_factor(table));

		printf("\nStart get data...\n\n");

		int get_fail_count = 0;

		for (int i = 0; i < VALUES_COUNT; i++) {
			void* f = ht_get(table, &keys[i], sizeof(keys[0]));

			if (!f) {
				printf("FAIL (NULL) on key: %d\n", keys[i]);

				get_fail_count++;
			} else if( *((int*) f) != values[i] ) {
				printf("FAIL (nor equal) on key: %d\n", keys[i]);

				get_fail_count++;
			}
		}

		printf("\nEND get data\n");

		printf("\nFail count: %d\n", get_fail_count);
		printf("Success count: %d\n", VALUES_COUNT - get_fail_count);

		if (get_fail_count) {
			printf("\nToo many fails\n");

			delete_ht(table);

			printf(END_OF_TEST);

			return 0;
		}
	} // END TEST BLOCK 1

	// TEST BLOCK 2: Recreate keys and try get values again

	printf("\nRecreate keys... ");

	int keys[VALUES_COUNT];

	for (int i = 0; i < VALUES_COUNT; i++) {
		keys[i] = i;
	}

	printf("OK\n");

	printf("\nStart get data again...\n\n");

	int get_fail_count = 0;

	for (int i = 0; i < VALUES_COUNT; i++) {
		void* f = ht_get(table, &keys[i], sizeof(keys[0]));

		if (!f) {
			printf("FAIL (NULL) on key: %d\n", keys[i]);

			get_fail_count++;
		} else if( *((int*) f) != values[i] ) {
			printf("FAIL (nor equal) on key: %d\n", keys[i]);

			get_fail_count++;
		}
	}

	printf("\nEND get data\n");

	printf("\nFail count: %d\n", get_fail_count);
	printf("Success count: %d\n", VALUES_COUNT - get_fail_count);

	if (get_fail_count) {
		printf("\nToo many fails\n");

		delete_ht(table);

		printf(END_OF_TEST);

		return 0;
	}

	// END TEST BLOCK 2

	printf("\nHt len: %llu\n", ht_size(table));
	printf("Ht load factor: %f\n", ht_load_factor(table));

	delete_ht(table);

	printf(END_OF_TEST);

	return 0;
}

#undef VALUES_COUNT

#define VALUES_COUNT 100

int HT_duplicate_test(void) {
	printf("\nStart: " str(HT_duplicate_test) "\n");

	int initial_capacity = 5;

	printf("\nInitial capacity: %d\n", initial_capacity);
	printf("Values count: %d\n", VALUES_COUNT);

	printf("\nCreate table... ");

	ht_t* table = ht_create(initial_capacity);

	if (!table) {
		printf("FAIL\n");

		printf(END_OF_TEST);
	}

	printf("OK\n");

	printf("Create data... ");

	int keys[VALUES_COUNT];
	int values[VALUES_COUNT];

	for (int i = 0; i < VALUES_COUNT; i++) {
		keys[i] = i;
		values[i] = i;
	}

	printf("OK\n");

	printf("\nAppend data... ");

	for (int i = 0; i < VALUES_COUNT; i++) {
		int code;

		code = ht_put(table, &keys[i], sizeof(keys[0]), &values[i]);

		if (code != 0) {
			printf("\n\nFAIL on iteration: %d\n", i);
			printf("Code: %d\n", code);

			delete_ht(table);

			printf(END_OF_TEST);

			return 0;
		}
	}

	printf("OK\n");

	printf("\nHt len: %llu\n", ht_size(table));
	printf("Ht load factor: %f\n", ht_load_factor(table));

	printf("\nStart get and change data...\n\n");

	int get_fail_count = 0;

	for (int i = 0; i < VALUES_COUNT; i++) {
		void* f = ht_get(table, &keys[i], sizeof(keys[0]));

		if (!f) {
			printf("FAIL (NULL) on key: %d\n", keys[i]);

			get_fail_count++;
		} else if( *((int*) f) != values[i] ) {
			printf("FAIL (nor equal) on key: %d\n", keys[i]);

			get_fail_count++;
		}
	}

	printf("\nEND get data\n");

	printf("\nFail count: %d\n", get_fail_count);
	printf("Success count: %d\n", VALUES_COUNT - get_fail_count);

	if (get_fail_count) {
		printf("\nToo many fails\n");

		delete_ht(table);

		printf(END_OF_TEST);

		return 0;
	}

	printf("\nChange data in table...\n\n");

	int new_values[VALUES_COUNT];
	int put_fail_count = 0;
	get_fail_count = 0;

	for (int i = 0; i < VALUES_COUNT; i++) {
		new_values[i] = VALUES_COUNT + i + 1;

		int code = ht_put(table, &keys[i], sizeof(keys[0]), &new_values[i]);

		if (code != 1) {
			printf("UNCORRECTED code: %d on iteration %i\n", code, i);

			put_fail_count++;
		}

		void* f = ht_get(table, &keys[i], sizeof(keys[0]));

		if (!f) {
			printf("FAIL (NULL) on key: %d\n", keys[i]);

			get_fail_count++;
		} else if( *((int*) f) != new_values[i] ) {
			printf("FAIL (nor equal) on key: %d\n", keys[i]);

			get_fail_count++;
		}
	}

	printf("\nEND change data in table\n");

	printf("\nPut fail count: %d\n", put_fail_count);
	printf("Success put count: %d\n", VALUES_COUNT - put_fail_count);

	printf("\nGet fail count: %d\n", get_fail_count);
	printf("Success get count: %d\n", VALUES_COUNT - get_fail_count);

	printf("\nHt len: %llu\n", ht_size(table));
	printf("Ht load factor: %f\n", ht_load_factor(table));

	delete_ht(table);

	printf(END_OF_TEST);

	return 0;
}
