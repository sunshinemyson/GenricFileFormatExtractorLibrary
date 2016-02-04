/**
 * @file FileFormat.cpp
 * @Brief implement basic work follow of abstract binary file
 * @author Sven Zhang
 * @version 0.01
 * @date 2016-02-03
 */

#include "FileFormat.hpp"
#include "common.h"

using namespace IO;

namespace BinExtract{
  
    AbstractFile::AbstractFile()
    {
        init();
    }

    AbstractFile::AbstractFile( ifstream* ifs )
    {
        init();
        mInputStream = ifs;
        mCurReadPos = ifs->tellg();
        // TODO : add support to get stream size
    }

    AbstractFile::AbstractFile( ofstream* ofs )
    {
        init();
        mOutputStream = ofs;
        mCurWritePos = ofs->tellp();
        // TODO : add support to get stream size
    }

    AbstractFile::~AbstractFile()
    {
      if( mInputStream )
      {
        delete mInputStream;
      }
      if( mOutputStream )
      {
        delete mOutputStream;
      }
    }

    void AbstractFile::init()
    {
        mFileSize       = -1;
        mCurReadPos     = -1;
        mCurWritePos    = -1;
        mInputStream    = NULL;
        mOutputStream   = NULL;

    }

    AbstractFile& AbstractFile::operator<<( iFileSection* aFileSec )
    {
        assert( NULL != aFileSec );

         mSectors.push_back( aFileSec );

        return *this;
    }

    int AbstractFile::rseekTo( int aOffset, SEEK_OP aSeekMode )
    {
        if( !mInputStream ) return -1;
        if( 0 == aOffset && mInputStream ) return mInputStream->tellg();

        switch( aSeekMode )
        {
            case SEEK_F_START:
                mInputStream->seekg( aOffset, io_base::beg );
                break;
            case SEEK_F_END:
                mInputStream->seekg( aOffset, io_base::end );
                break;
            case SEEK_F_CUR:
                mInputStream->seekg( aOffset, io_base::cur );
                break;
            default:
                return -1;
        }

        mCurReadPos = mInputStream->tellg();

        return -1;
    }

    int AbstractFile::wseekTo( int aOffset, SEEK_OP aSeekMode )
    {
        if( !mOutputStream ) return -1;
        if( 0 == aOffset && mOutputStream ) return mOutputStream->tellp();

        switch( aSeekMode )
        {
            case SEEK_F_START:
                mOutputStream->seekp( aOffset, io_base::beg );
                break;
            case SEEK_F_END:
                mOutputStream->seekp( aOffset, io_base::end );
                break;
            case SEEK_F_CUR:
                mOutputStream->seekp( aOffset, io_base::cur );
                break;
            default:
                return -1;
        }

        mCurWritePos = mOutputStream->tellp();

        return -1;
    }

    int AbstractFile::read( istream* input_stream )
    {
        int readBytes = -1;

        if( input_stream != NULL )
        {
            if( mInputStream && mInputStream != input_stream )
            {// delete old stream
                delete mInputStream;
            }
            mInputStream = input_stream;
            mCurReadPos = mInputStream->tellg();
            // TODO : add support to get stream size
            // mFileSize = ?
        }

        if( NULL == mInputStream )
        {
            // Nothing will happen because no input stream
            assert( false );
        }
        else
        {
            // input stream is ready to read
            readBytes = 0;
            for( SEC_LIST::iterator it = mSectors.begin(); it != mSectors.end(); ++it )
            {
                readBytes += (*it)->read( mInputStream );
            }
            mSectors.clear();
        }
        return readBytes;
    }

    int AbstractFile::write( ostream* output_stream )
    {
        int writeBytes = -1;

        if( NULL != output_stream )
        {
            if( mOutputStream && mOutputStream != output_stream )
            {
                 delete mOutputStream;
            }

            mOutputStream = output_stream;
            mCurWritePos  = mOutputStream->tellp();
            // TODO add support to get stream size
        }

        if( NULL == mOutputStream )
        {
             // Nothing will happen because no output stream
             assert( false );
        }
        else
        {
            writeBytes = 0;
            for( SEC_LIST::iterator it = mSectors.begin(); it != mSectors.end(); ++it )
            {
                writeBytes += (*it)->write( mOutputStream );
            }

            mSectors.clear();
        }

        return writeBytes;
    }
}
