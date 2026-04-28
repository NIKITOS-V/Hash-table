#ifndef __DECLARE_DATA_BLOCK_H__
#define __DECLARE_DATA_BLOCK_H__

typedef struct DataBlock DataBlock;

typedef void (DataBlock_destructor)(DataBlock* data_block);

#endif // __DECLARE_DATA_BLOCK_H__
