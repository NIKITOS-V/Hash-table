#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "local_tests.h"

int main() {
	init_tests();

	return do_test(HT_DUPLICATE_TEST);
}
