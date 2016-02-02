#include "StorageAccess.h"

#include "gtest/gtest.h"

TEST( StorageAbstractLayer, open_close )
{
   Ptr_Storage_Desc storage_ptr;

   storage_ptr = NULL;

   SAL_Open( "GoogleTest.bin", &storage_ptr );
   EXPECT_TRUE( storage_ptr != NULL ) << "Open Storage fail";

   SAL_Close( &storage_ptr );
   EXPECT_TRUE( storage_ptr == NULL ) << "Close storage fail";

   SAL_Open( "Nand_ImageBinary", &storage_ptr );
   EXPECT_TRUE( storage_ptr != NULL ) << "Open Nand Storage fail";

   SAL_Close( &storage_ptr );
   EXPECT_TRUE( storage_ptr == NULL ) << "Close Nand Storage fail";
}

TEST( StorageAbstractLayer, read_write )
{
    Ptr_Storage_Desc storage_ptr;
    int fd = open( "GoogleTest.bin", O_EXCL | O_CREAT );
    if( -1 == fd )
    {
        fd = open( "GoogleTest.bin", O_TRUNC );
        close( fd );
    }

    SAL_Open( "GoogleTest.bin", &storage_ptr );
    EXPECT_TRUE( storage_ptr != NULL ) << "Open Storage fail";

    const unsigned int      buf_sz          = 1024U;
    unsigned char*          p_write_buf     = new unsigned char[buf_sz];
    unsigned char*          p_read_buf      = new unsigned char[buf_sz];
    unsigned int            rslt_sz         = 0U;

    EXPECT_EQ( storage_ptr->backend_type, FILE_TYPE );

    for( unsigned int i = 0U; i < buf_sz; ++i )
    {
        *( p_write_buf + i ) = (unsigned char)(i % 0xFF);
    }

    rslt_sz = SAL_Write( &storage_ptr, 0, (void*)p_write_buf, buf_sz );
    EXPECT_EQ( buf_sz, rslt_sz );

    SAL_Close( &storage_ptr );
    SAL_Open( "GoogleTest.bin" ,&storage_ptr );

    rslt_sz = SAL_Read( &storage_ptr, 0, (void*)p_read_buf, buf_sz );
    EXPECT_EQ( buf_sz, rslt_sz );

    EXPECT_TRUE( memcmp( p_write_buf, p_read_buf, buf_sz ) == 0 ) << "content not equal to just writed";

    rslt_sz = SAL_Read( &storage_ptr, buf_sz, (void*)p_read_buf, buf_sz );
    EXPECT_EQ( 0, rslt_sz );

    rslt_sz = SAL_Read( &storage_ptr, 0, (void*)p_read_buf, 0 );
    EXPECT_EQ( 0, rslt_sz );

    rslt_sz = SAL_Read( &storage_ptr, buf_sz+1, (void*)p_read_buf, 1 );
    EXPECT_EQ( 0, rslt_sz );

    rslt_sz = SAL_Read( &storage_ptr, 0, NULL, 1 );
    EXPECT_EQ( -1, rslt_sz );           // because NULL for readbuf is invalid, so -1 returned

    rslt_sz = SAL_Write( &storage_ptr, 0, NULL, 0 );
    EXPECT_EQ( 0, rslt_sz );

    rslt_sz = SAL_Read( &storage_ptr, 0, (void*)p_read_buf, buf_sz );
    EXPECT_EQ( buf_sz, rslt_sz );
    EXPECT_TRUE( memcmp( p_write_buf, p_read_buf, buf_sz ) == 0 ) << "content not equal to just writed";

    for( unsigned int i = 0U; i < buf_sz; ++i )
    {
        *( p_write_buf + i ) = 0xFFU - (unsigned char)(i % 0xFFU);
    }

    rslt_sz = SAL_Write( &storage_ptr, 0, (void*)p_write_buf, buf_sz );
    EXPECT_EQ( buf_sz, rslt_sz );

    rslt_sz = SAL_Read( &storage_ptr, 0, (void*)p_read_buf, buf_sz );
    EXPECT_EQ( buf_sz, rslt_sz );

    EXPECT_TRUE( memcmp( p_write_buf, p_read_buf, buf_sz ) == 0 ) << "content not equal to just wirted";

    rslt_sz = SAL_Write( &storage_ptr, buf_sz, p_write_buf, buf_sz );
    EXPECT_EQ( buf_sz, rslt_sz );

    rslt_sz = SAL_Write( &storage_ptr, buf_sz, p_write_buf, 0 );
    EXPECT_EQ( 0, rslt_sz );

    rslt_sz = SAL_Write( &storage_ptr, 0, NULL, 0 );
    EXPECT_EQ( 0, rslt_sz );

    delete[] p_write_buf;
    delete[] p_read_buf;

    SAL_Close( &storage_ptr );
    EXPECT_TRUE( storage_ptr == NULL ) << "Close storage fail";
}


