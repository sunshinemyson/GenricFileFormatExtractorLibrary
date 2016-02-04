/**
 * @file StorageAccess.c
 * @Brief Storage Access Implementation
 * @author Sven Zhang
 * @version 0.01
 * @date 2016-02-01
 */

#include "StorageDescriptor.h"
#include "StorageAccess.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>

Storage     SystemStorageTable[] = {
    { "Nand_ImageBinary", { NAND_TYPE, { { 0x00000000U  , 4*1024*2014U  } } } }
   ,{ "GoogleTest.bin", { FILE_TYPE, { { 0U           , 0U            } } } }
   ,{ "img.bin", { FILE_TYPE, { { 0U, 0U }}}}
};

static const int  SystemStorageAmount = cnt_of_array( SystemStorageTable );
static const void* SystemStorageBegin = (void*)SystemStorageTable;
static const void* SystemStorageEnd   = (void*)((char*)SystemStorageTable + sizeof(SystemStorageTable));

#define IS_HANDLE_VALID( _S_HNDL ) ( (_S_HNDL) && ( SystemStorageBegin <= *(_S_HNDL) && *(_S_HNDL) < SystemStorageEnd ) )

//          TODO local API for backend
static int file_get_size( const char* fname );
static int file_read ( const char* fname, unsigned int r_offset, void* p_read_buffer, unsigned int buf_sz_B )
{
    int    read_size;
    int             fd;
    int             op_rslt;

    op_rslt     = -1;
    read_size   = 0U;
    fd          = -1;

    assert( fname );
    if(  0 == buf_sz_B )
    {
        return 0;
    }

    fd = open( fname, O_RDONLY );

    if( -1 == fd )
    {
        printf("check file %s exist before read\n", fname );
        getchar();
    }

    op_rslt = lseek( fd, r_offset, SEEK_SET );

    if( -1 == op_rslt )
    {
         printf("seek opearation fail\n");
    }

    read_size = read( fd, p_read_buffer, buf_sz_B );
    if( 0 == read_size )
    {
         // printf("End of file\n");;
    }
    else if( -1 == read_size )
    {
        printf( "file io error\n" );
    }

    close( fd );

    return read_size;
}

static int file_write( const char* fname, unsigned int w_offset, void* p_write_buffer, unsigned int buf_sz_B )
{
    int    write_size;
    int             fd;
    int             op_rslt;

    op_rslt         = -1;
    write_size      = 0U;
    fd              = -1;

    assert( fname );

    chmod( fname, S_IRWXU | S_IRWXG | S_IRWXO );
    fd = open( fname, O_WRONLY | O_CREAT );

    if( -1 == fd )
    {
        printf(" Cannot open %s for write\n", fname);
        printf(" error -> %s\n", strerror( errno ) );
        return -1;
    }

    op_rslt = lseek( fd, w_offset, SEEK_SET );

    if( -1 == op_rslt )
    {
         printf(" fail seek in file %s\n", fname);
         return -1;
    }

    write_size = write( fd, p_write_buffer, buf_sz_B );

    if( -1 == write_size )
    {
         printf(" error while write %s\n", fname );
    }

    close( fd );

    return write_size;
}
static int file_get_size( const char* fname )
{
    struct stat fstat ;

    memset( &fstat, 0, sizeof(struct stat) );

    stat( fname, &fstat );

    return fstat.st_size;
}
//

void SAL_Open( const char* the_storage_id, STORAGE_HANDLE the_storage_handle )
{
    int idx;

    assert( the_storage_id != NULL );
    assert( the_storage_handle != NULL );
    assert( !IS_HANDLE_VALID( the_storage_handle ) );

    for( idx = 0; idx < SystemStorageAmount; ++idx )
    {
         if( 0 == strcmp( the_storage_id, SystemStorageTable[idx].s_id ) )
         {
            break;
         }
    }

    if( idx != SystemStorageAmount )
    {
        *( the_storage_handle ) = &( SystemStorageTable[idx].storage_desc );
        if( FILE_TYPE ==  ( *the_storage_handle )->backend_type )
        {
            ( *the_storage_handle )->backend_info.file.pFileName = the_storage_id;
        }
    }
    else
    {
         *the_storage_handle = NULL;
    }
}

void SAL_Close( STORAGE_HANDLE aHandle )
{
    assert( IS_HANDLE_VALID( aHandle ) );

    // add clean up code if needed
    *aHandle = NULL;
}

int SAL_Read( STORAGE_HANDLE aHandle, unsigned int r_offset, void* p_read_buffer, unsigned int buf_sz_B )
{
    int        read_bytes;
    Ptr_Storage_Desc    ptr_storage;

    read_bytes = 0U;
    assert( IS_HANDLE_VALID( aHandle ) );
    ptr_storage = *aHandle;

    // debug code
    // printf("%s( .., offset = %d, p_read_buffer->0x%x, buf_sz = %d)\n" , __FUNCTION__,  r_offset, (p_read_buffer), buf_sz_B );

    if( NULL == p_read_buffer )
    {
        //client shouldn't pass an empty buffer to me, this cause error while access storage
        return -1;
    }

    if( 0 == buf_sz_B )
    {
       return 0;
    }

    switch( ptr_storage->backend_type )
    {
        case NAND_TYPE:
            break;
        case FILE_TYPE:
            read_bytes = file_read( ptr_storage->backend_info.file.pFileName, r_offset, p_read_buffer, buf_sz_B );
            break;
        default:
            read_bytes = -1;
            break;
    }

    return read_bytes;
}

int SAL_Write( STORAGE_HANDLE aHandle, unsigned int w_offset, void* p_write_buffer, unsigned int buf_sz_B )
{
    int        write_bytes;
    Ptr_Storage_Desc    ptr_storage;

    write_bytes = 0U;
    assert( IS_HANDLE_VALID( aHandle ) );
    ptr_storage = *aHandle;

    switch( ptr_storage->backend_type )
    {
        case NAND_TYPE:
            write_bytes = -1;
            break;
        case FILE_TYPE:
            write_bytes = file_write( ptr_storage->backend_info.file.pFileName, w_offset, p_write_buffer, buf_sz_B );
            break;
        default:
            write_bytes = -1;
            break;
    }

     return write_bytes;
}

int SAL_GetSize( STORAGE_HANDLE aHandle )
{
     int sz;
    Ptr_Storage_Desc    ptr_storage;

    sz = -1;
    assert( IS_HANDLE_VALID( aHandle ) );
    ptr_storage = *aHandle;

     switch( ptr_storage->backend_type )
    {
        case NAND_TYPE:
            sz = ptr_storage->backend_info.nand.size_in_bytes;
            break;
        case FILE_TYPE:
            sz = file_get_size( ptr_storage->backend_info.file.pFileName );
            break;
        default:
            break;
    }

    return sz;
}
