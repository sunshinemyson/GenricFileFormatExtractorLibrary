/**
 * @file StorageAccess.c
 * @Brief Storage Access Implementation
 * @author Sven Zhang
 * @version 0.01
 * @date 2016-02-01
 */

#include "StorageDescriptor.h"
#include "StorageAccess.h"

const Storage[]     SystemStorageTable = {
    { "Nand_ImageBinary", { NAND_TYPE, { 0x00000000U, 4*1024*2014U } } }
   ,{ "./GoogleText.bin", { FILE_TYPE, { "./GoogleText.bin" } } }
};

void SAL_Open( const char* the_stroage_id, STORAGE_HANDLE the_storage_handle )
{
    // STUB TODO
}

void SAL_Close( STORAGE_HANDLE aHandle )
{
     // STUB TODO
}

int SAL_Read( STORAGE_HANDLE aHandle, unsigned int r_offset, void* p_read_buffer, unsigned int buf_sz_B )
{
    // STUB TODO
     return -1;
}

int SAL_Write( STORAGE_HANDLE aHandle, unsigned int w_offset, void* p_write_buffer, unsigned int buf_sz_B )
{
    // STUB TODO
     return -1;
}
