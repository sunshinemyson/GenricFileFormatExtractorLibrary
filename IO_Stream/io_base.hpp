/**
 * @file io_base.hpp
 * @Brief io interface
 * @author Sven Zhang
 * @version 0.01
 * @date 2016-02-01
 */

#ifndef IO_BASE_HPP
#define IO_BASE_HPP

#include "StorageDescriptor.h"
#include <assert.h>

/* ------------------------------------------------------------*/
/**
 * @Brief classes under IO namespace will implement a sub-set API of std::io
 */
/* ------------------------------------------------------------*/
namespace IO{

    typedef int streamoff;
    typedef int streamsize;
    typedef int streampos;

    class  io_base{
        public:
            typedef enum {
                beg,
                cur,
                end
            } seek_dir;

            typedef enum{
                in,
                out,
                binary,
                ate,        // at end
                app,
                trunc
            } openmode;

            io_base()
                : mSize( 0 )
                  ,mOffset( 0 )
            {}

            virtual bool eof()
            {
                return (mOffset >= mSize);
            }

            virtual streampos tellg()
            {
                return mOffset;
            }

            virtual io_base& seekg( streamoff aOffset, io_base::seek_dir way )
            {
                switch ( way )
                {
                    case io_base::beg:
                        mOffset = static_cast<streampos>( aOffset );
                        break;
                    case io_base::cur:
                        mOffset += static_cast<streampos>( aOffset );
                        break;
                    case io_base::end:
                        mOffset = mSize - 1 + static_cast<streampos>( aOffset );
                        break;
                    default:
                        assert( false );
                        break;
                }

                if( mOffset < 0 )
                {
                     mOffset = 0;
                }

                if( mOffset > mSize )
                {
                     mOffset = mSize;
                }

                return *this;
            }
        protected:
            streamsize      mSize;          //!< the input stream's size
            streampos       mOffset;        //!< the current input offset
    };

    class istream : public io_base
    {
        public:
            virtual istream& read( char* s, streamsize n ) = 0;
    };

    class ostream : public io_base
    {
        public:
            virtual ostream& write( char* s, streamsize n ) = 0;
    };

    class ifstream : public istream
    {
        public:
            explicit ifstream( const char* aFileName, io_base::openmode mode = io_base::in );
            virtual ~ifstream();

            /* ------------------------------------------------------------*/
            /**
             * @Brief read content from stream(through storage)
             *
             * @Param buf : Never pass NULL, or it will be treated an error then steam will be eof state
             * @Param n : don't give a negative value which could be convert to very large positive value
             *
             * @return istream with state updated
             */
            /* ------------------------------------------------------------*/
            virtual istream& read( char* buf, streamsize n );

        private:
            Ptr_Storage_Desc     mStorage;
    };

    class ofstream : public ostream
    {
        public:
            explicit ofstream( const char* aFileName, io_base::openmode mode = io_base::out );
            virtual ~ofstream();

            /* ------------------------------------------------------------*/
            /**
             * @Brief write content of buf to file
             * @Detail
             *      NOTE this operation MUST keep file size not changed
             *      if storage don't have enough space to save all the content of buffer,
             *      the content will be cut off to fit file size
             *
             * @Param buf: byte buffer to write out
             * @Param n: count of bytes
             *
             * @return the stream instance contain the updated state(mOffset updated)
             */
            /* ------------------------------------------------------------*/
            virtual ostream& write( char* buf, streamsize n );

        private:
            Ptr_Storage_Desc     mStorage;
    };
}

#endif

