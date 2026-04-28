cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Hash_table")

include (Sources.cmake)

include_directories (
	${GENERIC_TYPES_DIR}
	${COMMON_DIR}
	${ARRAY_AVL_TREES_DIR}
  	${AVL_NODE_DIR}
	${AVL_TREE_DIR}
  	${DATA_BLOCK_DIR}
	${HASH_FUNCTIONS_DIR}
  	${HASH_TABLE_DIR}
	${LOCAL_TESTS_DIR}
	${TEST_DIR}
)

add_executable (Hash_table
	#src/main.c
	${COMMON_SOURCES}
  	${ARRAY_AVL_TREES_SOURCES}
  	${AVL_NODE_SOURCES}
  	${AVL_TREE_SOURCES}
  	${DATA_BLOCK_SOURCES}
	${HASH_FUNCTIONS_SOURCES}
  	${HASH_TABLE_SOURCES}
	${LOCAL_TESTS_SOURCES}
	${TEST_SOURCES}
)

include (Warnings.cmake)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Hash_table PROPERTY C_STANDARD 99)
endif()
