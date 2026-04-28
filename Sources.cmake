# CMakeLists.txt for Hash_table project

# Directories

# Src and root directory definitions

set (ROOT_DIR "src/HashTable")
set (SRC "src")

# Common directories

set (GENERIC_TYPES_DIR "${SRC}/generic_types")
set (COMMON_DIR "${ROOT_DIR}/common")

# end

# Directory structure and source files

# Common

set (HASH_FUNCTIONS_DIR "${COMMON_DIR}/hash_functions")

# Arrays

set (ARRAY_AVL_TREES_DIR "${ROOT_DIR}/Arrays/Array_AVLTrees")

# AVL Tree & AVL Node

set (AVL_TREE_DIR "${ROOT_DIR}/AVLTree")
set (AVL_NODE_DIR "${AVL_TREE_DIR}/AVLNode")

# Hash Table & Data BLock

set (DATA_BLOCK_DIR "${ROOT_DIR}/DataBlock")
set (HASH_TABLE_DIR "${ROOT_DIR}")

# Test directory

set (TESTS_DIR "${SRC}/test")
set (LOCAL_TESTS_DIR "${TESTS_DIR}/local_tests")

# end

# Set sources

# Common

set (COMMON_SOURCES
	"${COMMON_DIR}/compare.c"
)

set (HASH_FUNCTIONS_SOURCES
	"${HASH_FUNCTIONS_DIR}/SDBM.c"
)

# Arrays

set (ARRAY_AVL_TREES_SOURCES
	"${ARRAY_AVL_TREES_DIR}/Array_AVLTrees.c"
)

# AVL Tree & AVL Node

set (AVL_TREE_SOURCES
	"${AVL_TREE_DIR}/AVLTree.c"
)

set (AVL_NODE_SOURCES
	"${AVL_NODE_DIR}/AVLNode.c"
)

# Hash Table & Data Block

set (DATA_BLOCK_SOURCES
	"${DATA_BLOCK_DIR}/DataBlock.c"
)

set (HASH_TABLE_SOURCES
	"${HASH_TABLE_DIR}/HashTable.c"
)

# Test source files

set (LOCAL_TESTS_SOURCES
	"${LOCAL_TESTS_DIR}/HT_tests.c"
	"${LOCAL_TESTS_DIR}/AVLTree_tests.c"
	"${LOCAL_TESTS_DIR}/Tests_list.c"
)

set (TEST_SOURCES
	"${TESTS_DIR}/tests_hash.c"
)

# end
