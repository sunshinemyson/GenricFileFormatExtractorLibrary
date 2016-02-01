/**
 * @file basic_io.hpp
 * @Brief io interface
 * @author Sven Zhang
 * @version 0.01
 * @date 2016-02-01
 */

#ifndef BASIC_IO_HPP
#define BASIC_IO_HPP

/* ------------------------------------------------------------*/
/**
 * @Brief classes under IO namespace will implement a sub-set API of std::ios
 */
/* ------------------------------------------------------------*/
namespace IO{

    typedef int streamoff;
    typedef int streamsize;

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

            virtual bool eof() = 0;

        protected:
            streamsize      mSize;          //!< the input stream's size
            streampos       mOffset;        //!< the current input offset
    };

    class istream : public io_base
    {
        public:

            istream()
                : mSize( 0 )
                  ,mOffset( 0 )
            {
            }

            virtual streampos tellg()
            {
                return mOffset;
            }

            virtual istream& seekg( streamoff aOffset, io_base::seek_dir way )
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

                return *this;
            }

            virtual istream& read( char* s, streamsize n ) = 0;

            virtual bool eof()
            {
                return (mOffset >= mSize);
            }
    };

    class ostream : public io_base
    {
        public:
            virtual streampos tellg()
            {
                return mOffset;
            }

            virtual ostream& seekg( streamoff aOffset, io_base::seek_dir way )
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

                return *this;
            }

            virtual int wirte( char* s, streamsize n ) = 0;
    };

    class ifstream : public istream
    {
        public:
            explicit ifstream( const char* aFileName, ios_base::openmode mode = io_base::in );
    };

    class ofstream : public ostream
    {
        public:
            explicit ofstream( const char* aFileName, ios_base::openmode mode = io_base::out );
    };
}

#endif

