/**
 * @file StorageAccess.h
 * @Brief Storage abstraction layer
 * @author Sven Zhang
 * @version 0.01
 * @date 2016-01-27
 */

# ifndef STORAGEACCESS_H
# define STORAGEACCESS_H

#include "SAL_config.h"

# include "StorageDescriptor.h"

CPLUSPLUS_BEGIN

typedef enum{
    SAL_SUCCESS,
    SAL_EOF,
    SAL_UNKNOW_ERROR,

    SAL_STATUS_CODE_CNT
} SAL_STATE;

/* ------------------------------------------------------------*/
/**
 * @Brief Open given storage
 *
 * @Param aStorageBackEndID : the storage's unique string
 * @Param the_sotrage_handle : the storage handle, if failed to found, NULL returned
 *
 */
/* ------------------------------------------------------------*/
void            SAL_Open    ( const char* aStorageBackEndID, STORAGE_HANDLE the_sotrage_handle );

/* ------------------------------------------------------------*/
/**
 * @Brief Close given storage handle
 *
 * @Param the_storage_handle : the storage handle closed
 *      will reset it to NULL
 *
 */
/* ------------------------------------------------------------*/
void            SAL_Close   ( STORAGE_HANDLE  the_storage_handle );

/* ------------------------------------------------------------*/
/**
 * @Brief Read data from storage
 *
 * @Param the_storage : data source, must be valid
 * @Param r_offset : read offset, always count from beginning of storage
 * @Param p_read_buffer : buffer to store data from storage
 * @Param buf_sz_B : total bytes require
 *
 * @return total bytes actually read, -1 stand for error, 0 means eof
 */
/* ------------------------------------------------------------*/
int    SAL_Read    ( STORAGE_HANDLE the_storage, unsigned int r_offset, void* p_read_buffer, unsigned int buf_sz_B );

/* ------------------------------------------------------------*/
/**
 * @Brief Write data to storage
 *
 * @Param the_storage : data target, must be valid
 * @Param w_offset : write offset, always count from beginning of storage
 * @Param p_write_buffer : buffer to write out
 * @Param buf_sz_B : total bytes want to write out
 *
 * @return total bytes actually write out, -1 stand for error
 */
/* ------------------------------------------------------------*/
int    SAL_Write   ( STORAGE_HANDLE the_storage, unsigned int w_offset, void* p_write_buffer, unsigned int buf_sz_B );

/* ------------------------------------------------------------*/
/**
 * @Brief get total size of current storage
 *
 * @Param the_storage handle
 *
 * @return size in bytes, -1 stand for no limited size
 */
/* ------------------------------------------------------------*/
int     SAL_GetSize( STORAGE_HANDLE the_storage );

/* ------------------------------------------------------------*/
/**
 * @Brief Get sal state after last operation executed
 *      the status my be mess under multi task
 *
 * @return the latest state of SAL module
 */
/* ------------------------------------------------------------*/
SAL_STATE       SAL_Get_Last_State( void );

CPLUSPLUS_END

# endif

