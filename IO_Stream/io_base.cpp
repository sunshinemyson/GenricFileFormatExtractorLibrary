/**
 * @file io_base.cpp
 * @Brief io_base implementation
 * @author Sven Zhang
 * @version 0.01
 * @date 2016-02-02
 */

#include "io_base.hpp"
#include "StorageAccess.h"

#include <stdio.h>

namespace IO
{
    ifstream::ifstream( const char* aFileName, io_base::openmode mode )
        : istream()
    {
        assert( aFileName );
        mStorage = NULL;

        SAL_Open( aFileName, &mStorage );
        mSize = SAL_GetSize( &mStorage );
        assert( NULL != mStorage );
    }

    ifstream::~ifstream()
    {
        SAL_Close( &mStorage );
    }

    istream& ifstream::read( char* buf, streamsize nbytes )
    {
        if( NULL != buf && nbytes > 0 )
        {
            int read_bytes = ( mOffset + nbytes >= mSize ) ? ( mSize - mOffset ) : nbytes;
            int read_cnt = SAL_Read( &mStorage, mOffset, buf, read_bytes );
            if( -1 != read_cnt )
            {
                //printf("DEBUG : read %d byte from file( %d )", read_cnt, nbytes );
                mOffset += read_cnt;
            }
            else if( -1 == read_cnt ){
                // error
                //printf("DEBUG : error at %s line %d : mSize = %d, mOffset = %d \n", __FILE__, __LINE__, mSize, mOffset );
                mOffset = mSize;
            }

        }
        return *this;
    }

    ofstream::ofstream( const char* aFileName, io_base::openmode mode )
    {
        assert( NULL != aFileName );
        mStorage = NULL;

        SAL_Open( aFileName, &mStorage );
        mSize = SAL_GetSize( &mStorage );
        assert( NULL != mStorage );
    }

    ofstream::~ofstream()
    {
        SAL_Close( &mStorage );
    }

    ostream& ofstream::write( char* buf, streamsize nbytes )
    {
        if( NULL != buf && nbytes >= 0 )
        {
            int write_bytes = ( mOffset + nbytes ) >= mSize ? mSize - mOffset : nbytes;
            int write_cnt = SAL_Write( &mStorage, mOffset, buf, write_bytes );
            if( write_cnt > 0 )
            {
                 mOffset += write_cnt;
            }
        }
        else
        {
            assert( false );
        }

        return *this;
    }
}

