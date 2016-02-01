/**
 * @file FileFormat.hpp
 * @Brief Definition releated to file format
 * @author Sven Zhang
 * @version 0.01
 * @date 2016-01-27
 */

namespace IO{
    class BaseIO;
    class istream;
    class ostream;
    class fistream;
    class foutstream;
}

using namespace IO::istream;
using namespace IO::ostream;

namespace BinExtract{

    class iFileSection {

        /* ------------------------------------------------------------*/
        /**
         * @Brief read data from input stream
         *
         * @Param the input stream which should be promised point to right position
         *
         * @return total bytes read from the input stream; return -1 stand for EOF
         */
        /* ------------------------------------------------------------*/
        virtual int read( istream* ) = 0;

        /* ------------------------------------------------------------*/
        /**
         * @Brief write data to output stream
         *
         * @Param the output stream should be promised in right position
         *
         * @return total bytes writeout by output stream; return -1 stand for EOF
         */
        /* ------------------------------------------------------------*/
        virtual int write( ostream* ) = 0;

        /* ------------------------------------------------------------*/
        /**
         * @Brief get current section size in bytes
         *
         * @return section size in bytes
         */
        /* ------------------------------------------------------------*/
        virtual int getSize() = 0;
    }

    template< typename T >
    class FileSection : public iFileSection {
        public:
            FileSection();

            virtual int read( istream* input_stream );

            virtual int write( ostream* output_stream );

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

            /* ------------------------------------------------------------*/
            /**
             * @Brief "<<" oprator to push a new filesection to the Abstract file
             *          you can geti your content via aFsection after AbstractFile execture
             *
             * @Param aFsection : the section to extract or write out
             *
             * @return *this
             */
            /* ------------------------------------------------------------*/
            AbstractFile& operator<<( const iFileSection* aFsection );

            typedef enum{
                 SEEK_F_START,
                 SEEK_F_END,
                 SEEK_F_CUR,

                 SEEK_INVALID
            } SEEK_OP;

            /* ------------------------------------------------------------*/
            /**
             * @Brief seek file with given offset
             *
             * @Param aOffset : seek value by bytes, negative value means seek backward or seek forward
             * @Param aOpCode : Seek mode, default is seek from curren pos
             *
             * @return the original file position, if you pass 0 for aOffset, you can get
             *      current pos without touch anything related the stream
             */
            /* ------------------------------------------------------------*/
            int seekTo( int aOffset, SEEK_OP aOpCode = SEEK_F_CUR );

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
             * @return totoal size read in by bytes
             */
            /* ------------------------------------------------------------*/
            virtual int read( istream* input_stream = NULL );

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
             * @return total bytes write out
             */
            /* ------------------------------------------------------------*/
            virtual int write( ostream* output_stream = NULL );

        private:
            int             mCurPos ;               //!< the default read/write offset in current file
            int             mFileSize;              //!< the file size

            istream         *mInputStream;          //!< the input stream used in read(); released by me
            ostream         *mOutputStream;         //!< the output stream used in write(); released by me

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
