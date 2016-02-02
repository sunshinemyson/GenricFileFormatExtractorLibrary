/**
 * @file StorageDescriptor.h
 * @Brief Storage Descriptor definition
 * @author Sven Zhang
 * @version 0.01
 * @date 2016-01-27
 */

# ifndef STORAGE_DESCRIPTOR_H
# define STORAGE_DESCRIPTOR_H

typedef enum{
    NAND_TYPE,
    FILE_TYPE,

    TYPE_CNT
} BACKEND_TYPE;

typedef struct _Storage_Desc
{
    BACKEND_TYPE    backend_type;

    union
    {

        struct backend_nand{

            // for nandflash type
            unsigned int start_address;             //!< nandflash start address
            unsigned int size_in_bytes;             //!< total size
        } nand;

        struct backend_file{
            // for Linux Platform
            const char* pFileName;                  //!< the file name with path
        } file;
    }backend_info;

}Storage_Desc, *Ptr_Storage_Desc;

typedef Ptr_Storage_Desc*   STORAGE_HANDLE;
typedef const char*         STORAGE_ID;

typedef struct _Storage{
    STORAGE_ID          s_id;
    Storage_Desc        storage_desc;
}Storage;

extern Storage SystemStorageTable[];
/*
{
    { "Nand_ImageBinary", { NAND_TYPE, { 0x00000000U, 4*1024*2014U } } }
   ,{ "./GoogleText.bin", { FILE_TYPE, { "./GoogleText.bin", NULL } } }
};
*/

# endif

