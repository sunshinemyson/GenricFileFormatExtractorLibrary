/**
 * @file FileFormat.hpp
 * @Brief Definition releated to file format
 * @author Sven Zhang
 * @version 0.01
 * @date 2016-01-27
 */

#include <deque>
#include <stdio.h>

#include "io_base.hpp"
#include "common.h"

namespace BinExtract{

    class iFileSection {
        public:

            /* ------------------------------------------------------------*/
            /**
             * @Brief read data from input stream
             *
             * @Param the input stream which should be promised point to right position
             *
             * @return total bytes read from the input stream; return -1 stand for EOF
             */
            /* ------------------------------------------------------------*/
            virtual int read( IO::istream* ) = 0;

            /* ------------------------------------------------------------*/
            /**
             * @Brief write data to output stream
             *
             * @Param the output stream should be promised in right position
             *
             * @return total bytes writeout by output stream; return -1 stand for EOF
             */
            /* ------------------------------------------------------------*/
            virtual int write( IO::ostream* ) = 0;

            /* ------------------------------------------------------------*/
            /**
             * @Brief get current section size in bytes
             *
             * @return section size in bytes
             */
            /* ------------------------------------------------------------*/
            virtual int getSize() = 0;
    };

    template< typename T >
    class FileSection : public iFileSection {
        public:
            typedef T value_type;

            explicit FileSection( const T& aData )
            {
                mData = aData;
            }

            FileSection()
            {
                memset( &mData, 0, sizeof(T) );
            }

            virtual ~FileSection(){}

            virtual int read( IO::istream* input_stream )
            {
                assert( NULL != input_stream );

                int readBytesCnt = 0;

                // TODO: stream doesn't feedback last read()'s result
                readBytesCnt = sizeof(T);

                input_stream->read( (char*)&mData, sizeof(mData) );

                return readBytesCnt;
            }

            virtual int write( IO::ostream* output_stream )
            {
                //printf("DEBUG : write to oupput_stream\n");
                assert( NULL != output_stream );

                int writeBytesCnt = 0;

                // TODO: stream doesn't feedback last write()'s result
                writeBytesCnt = sizeof(T);

                output_stream->write( (char*)&mData, sizeof(mData) );

                return writeBytesCnt;
            }

            virtual int getSize()
            {
                return sizeof( T );
            }

            T mData;                                        //!< templated data
    };

    class AbstractFile{
        public:

            typedef std::deque< iFileSection* >     SEC_LIST;
            typedef SEC_LIST::iterator              SEC_ITERATOR;

            AbstractFile();

            explicit AbstractFile( IO::ifstream * );

            explicit AbstractFile( IO::ofstream * );

            virtual ~AbstractFile();

            /* ------------------------------------------------------------*/
            /**
             * @Brief "<<" oprator to push a new filesection to the Abstract file
             *          you can geti your content via aFsection after AbstractFile execture
             *          after read/write operation done these sections will removed from internal list
             *
             * @Param aFsection : the section to extract or write out, never pass NULL
             *
             * @return *this
             */
            /* ------------------------------------------------------------*/
            AbstractFile& operator<<( iFileSection* aFsection );

            typedef enum{
                 SEEK_F_START,
                 SEEK_F_END,
                 SEEK_F_CUR,

                 SEEK_INVALID
            } SEEK_OP;

            /* ------------------------------------------------------------*/
            /**
             * @Brief seek input stream with given offset
             *
             * @Param aOffset : seek value by bytes, negative value means seek backward or seek forward
             * @Param aOpCode : Seek mode, default is seek from current pos
             *
             * @return the original file position, if you pass 0 for aOffset, you can get
             *      current pos without touch anything related the stream
             */
            /* ------------------------------------------------------------*/
            int rseekTo( int aOffset, SEEK_OP aSeekOp = SEEK_F_CUR );

            /* ------------------------------------------------------------*/
            /**
             * @Brief seek output stream with given offset
             *
             * @Param aOffset : seek value by bytes, negative value means seek backward  or seek forward
             * @Param aSeekOp : seek mode, default is seek from current pos
             *
             * @return the original stream position, if you pass 0 to aOffset, you'll get
             *  current pos without touch anything within stream
             */
            /* ------------------------------------------------------------*/
            int wseekTo( int aOffset, SEEK_OP aSeekOp = SEEK_F_CUR );

            /* ------------------------------------------------------------*/
            /**
             * @Brief read content from input_stream, you must provide aFileSection format
             *      by operator<< so that we can store the data in it/them
             *
             *      SYNC api
             *
             * @Param input_stream data source, client don't need free the input_stream's resource
             *      if client change the intput stream, the old one will be closed and freed automatically
             *      NULL : if client just want to read new section content, they shouldn't pass a new input stream
             *      None-NULL : the new input_stream
             *
             * @return totoal size read in by bytes, return -1 if error detected
             */
            /* ------------------------------------------------------------*/
            virtual int read( IO::istream* input_stream = NULL );

            /* ------------------------------------------------------------*/
            /**
             * @Brief write out content by output_stream, you must provide aFileSection
             *      content by operator<< so that we can write data in it to output
             *
             *      SYNC api
             *
             * @Param output_stream output , client don't need free output_stream's resource
             *      if client change the output_stram, the old one will be closed and freed automatically
             *      NULL : if client just want to write new section content, they should'nt pass a new output stream
             *      None-Null : the new output stream
             *
             * @return total bytes write out, return -1 if error detected
             */
            /* ------------------------------------------------------------*/
            virtual int write( IO::ostream* output_stream = NULL );

        protected:
            void            init();

            int             mCurReadPos ;               //!< the default read offset in current file
            int             mCurWritePos;               //!< the default write offset int current file
            int             mFileSize;              //!< the file size

            IO::istream         *mInputStream;          //!< the input stream used in read(); released by me
            IO::ostream         *mOutputStream;         //!< the output stream used in write(); released by me

            SEC_LIST        mSectors;               //!< the sectors pointers which used while reading/writing
    };

    /********************************************************************************
     * example :
     *  step 1 : you construct your file format ( make sure what content you want to read  )
     *      if our file originazed by following format: only two simple section
     *          {[int:32bits][char:8bits]}
     *      declare FileSectionInt: public FileSection< int >
     *              FileSectionChar: public FileSection< char >
     *  read example :
     *      2.1 ifstream* input_stream = new ifstream ( ... );
     *      2.2 AbstractFile theFile;                                   // construct the file
     *      2.3 FileSectionInt intValue;                                // int  section
     *      2.4 FileSectionChar charValue;                              // char section
     *      2.5 theFile<<intValue<<charValue;                           // let file know the sections will be read in/write out
     *      2.6 theFile.read( input_stream );
     *      2.7 std::cout<< *intValue << *charValue;        // you can get the values
     *
     *      or read char section directly.
     *      a. theFile.seekTo(  intValue.getSize() );
     *      b. theFile.read( input_stream );
     *
     * write example :
     *      3.1 ofstream* output_stream  = new ofstream( ...  );
     *      3.2 AbstractFile outFile( );
     *      3.3 FileSectionInt intValue;
     *      3.4 FileSectioncChar charValue;
     *      3.5 intValue.setValue( 0x1001 ), charValue.setValue( 'a' );;
     *      3.6 theFile<<intValue<<charValue;
     *      3.7 theFlie.write( output_stream );                                    // the file content will be binary of 0x1001 and 'a'
     *
    ********************************************************************************/
};
