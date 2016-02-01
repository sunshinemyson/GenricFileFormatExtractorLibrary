/**
 * @file ImageBinary.hpp
 * @Brief Image Binary format definition
 * @author Sven Zhang
 * @version 0.01
 * @date 2016-02-01
 */
#ifndef IMAGEBINARY_HPP
#define IMAGEBINARY_HPP

#include "FileFormat.hpp"

namespace IMG_BIN{

    typedef FileSection<char>   EndianSection;
    typedef FileSection<int>    FileSizeSection;

    typedef struct _image_entry_sec{
        int mBasicAttr;                 //!< Image basic attrbute( colorformat, width, height )
        int mAttrTile;                  //!< Image tile attribute( left_margin, right_margin, top_margin, bottom_margin )
        int mDataOffset;                //!< Image raw data's offset(int bytes) in file
    } Image_Entry_Section;

    typedef FileSection< Image_Entry_Section > ImageEntry;

    typedef struct _image_raw_data{
        char* mPtrRawData;              //!< allocated from heap
        unsigned int mSzBytes;          //!< total Size of raw data
    } Image_Raw_Data;


    /* ------------------------------------------------------------*/
    /**
     * @Brief ImageRawData section's structure need overwrite read/write
     * because it contain a heap resource need write out
     */
    /* ------------------------------------------------------------*/
    class ImageRawData : public FileSection< Image_Raw_Data > {
        public:
            explicit ImageRawData( unsigned int aSize );

            virtual ~ImageRawData();

            virtual int read( istream* input_stream );

            virtual int write( ostream* output_stream );

            virtual int getSize();
    };

    /* ------------------------------------------------------------*/
    /**
     * @Brief class stand for an Image Binary, which you can get
     *  an image's information by handle. Image's handle is the offset of
     *  image entry in the binary file
     */
    /* ------------------------------------------------------------*/
    class ImageBinFile : AbstractFile{
        public:

            ImageBinFile();

            inline unsigned int    getFileSz()
            {
                return mFileSize;
            }

            /* ------------------------------------------------------------*/
            /**
             * @Brief get image raw data with given handle
             *
             * @Param aHandle : the image entry's offset (in bytes)
             *
             * @return the Raw data structure which contain rgb data, client
             *  should promised release memory, NULL if image not found
             */
            /* ------------------------------------------------------------*/
            ImageRawData*   getImageData( const int aHandle );

            /* ------------------------------------------------------------*/
            /**
             * @Brief get image entry structure
             *
             * @Param aHandle : image entry's offset in file
             *
             * @return the ImageEntry need free by caller, NULL if not found
             */
            /* ------------------------------------------------------------*/
            ImageEntry*     getImageAttr( const int aHandle );

            /* ------------------------------------------------------------*/
            /**
             * @Brief get width of image
             *
             * @Param aHandle : image entry's offset in file
             *
             * @return width of image, 2^WIDTH_BITS if not found image
             */
            /* ------------------------------------------------------------*/
            unsigned int    getImageWidth( const int aHandle );

            /* ------------------------------------------------------------*/
            /**
             * @Brief get height of image
             *
             * @Param aHandle : image entry's offset in file
             *
             * @return aHandle : image entry's offset in file
             */
            /* ------------------------------------------------------------*/
            unsigned int    getImageHeight( const int aHandle );

            /* ------------------------------------------------------------*/
            /**
             * @Brief overload read because we want save filesize when input stream setup first time
             *
             * @Param input : the input stream
             *
             * @return actual size read from input stream
             */
            /* ------------------------------------------------------------*/
            virtual int     read( istream* input = NULL );

            /* ------------------------------------------------------------*/
            /**
             * @Brief overload write because we don't need write for now
             *
             * @Param ouput
             *
             * @return always -1 stand for not support
             */
            /* ------------------------------------------------------------*/
            virtual int     write( ostream* ouput = NULL );

        private:
            unsigned int    mFileSize;              //!< binary file size in bytes
    };
}

#endif

