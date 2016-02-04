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
#include "bitmask.h"

namespace IMG_BIN
{

    typedef BinExtract::FileSection<char>             EndianSection;
    typedef BinExtract::FileSection<unsigned int>     FileSizeSection;

    enum {
      ENU_IMG_WIDTH_OFST = 0,
      ENU_IMG_WIDTH_BITS = 13,
      ENU_IMG_WIDTH_MASK = NEW_BITMASK ( ENU_IMG_WIDTH_OFST, ENU_IMG_WIDTH_BITS ),

      ENU_IMG_HEIGHT_OFST = ENU_IMG_WIDTH_OFST + ENU_IMG_WIDTH_BITS,
      ENU_IMG_HEIGHT_BITS = 13,
      ENU_IMG_HEIGHT_MASK = NEW_BITMASK ( ENU_IMG_HEIGHT_OFST, ENU_IMG_HEIGHT_BITS ),

      ENU_IMG_COLOR_FORMAT_OFST = ENU_IMG_HEIGHT_OFST + ENU_IMG_HEIGHT_BITS,
      ENU_IMG_COLOR_FORMAT_BITS = 3,
      ENU_IMG_COLOR_FORMAT_MASK = NEW_BITMASK ( ENU_IMG_COLOR_FORMAT_OFST, ENU_IMG_COLOR_FORMAT_BITS ),
      ENU_IMG_COLOR_FORMAT_ONE = NEW_BITMASK ( ENU_IMG_COLOR_FORMAT_OFST, 1),
      ENU_IMG_COLOR_FORMAT_RGB888 = 0,
      ENU_IMG_COLOR_FORMAT_RGB565 = ENU_IMG_COLOR_FORMAT_RGB888 + ENU_IMG_COLOR_FORMAT_ONE,
      ENU_IMG_COLOR_FORMAT_ARGB8888 = ENU_IMG_COLOR_FORMAT_RGB565 + ENU_IMG_COLOR_FORMAT_ONE,
      // add new color format CONSTANTS value here
    };

    struct ImgGetWidth {
      int operator()( int maskedValue )
      {
        return ((maskedValue & ENU_IMG_WIDTH_MASK) >> ENU_IMG_WIDTH_OFST);
      }
    };

    struct ImgGetHeight {
      int operator()( int maskedValue )
      {
        return ((maskedValue & ENU_IMG_HEIGHT_MASK) >> ENU_IMG_HEIGHT_OFST);
      }
    };

    struct RGB888{};
    struct RGB565{};
    struct ARGB8888{};
    template< typename T_COLOR_FORMAT >
    struct ImageSize
    {
        static const unsigned int PixelSize = 0U; //Bytes;
    };

    template<> struct ImageSize<RGB888> {
      static const unsigned int PixelSize = 3U;   //Bytes
    };

    template<> struct ImageSize<RGB565> {
      static const unsigned int PixelSize = 2U;   //Bytes
    };

    template<> struct ImageSize<ARGB8888> {
      static const unsigned int PixelSize = 4U;   //Bytes
    };

    typedef struct _basic_attr{
      unsigned int mWidth;
      unsigned int mHeight;
      unsigned int mColorFormat;
    } BasicAttr;

    typedef struct _tile_attr{
      unsigned int mLeftMargin;
      unsigned int mRightMargin;
      unsigned int mTopMargin;
      unsigned int mBottomMargin;
    } TileAttr;

    enum {
        ENU_L_MARGIN_OFST,
        ENU_L_MARGIN_BITS = 8,
        ENU_L_MARGIN_MASK = NEW_BITMASK ( ENU_L_MARGIN_OFST, ENU_L_MARGIN_BITS ),

        ENU_R_MARGIN_OFST = ENU_L_MARGIN_OFST + ENU_L_MARGIN_BITS,
        ENU_R_MARGIN_BITS = 8,
        ENU_R_MARGIN_MASK = NEW_BITMASK ( ENU_R_MARGIN_OFST, ENU_R_MARGIN_BITS ),

        ENU_T_MARGIN_OFST = ENU_R_MARGIN_OFST + ENU_R_MARGIN_BITS,
        ENU_T_MARGIN_BITS = 8,
        ENU_T_MARGIN_MASK = NEW_BITMASK ( ENU_T_MARGIN_OFST, ENU_T_MARGIN_BITS ),

        ENU_B_MARGIN_OFST = ENU_T_MARGIN_OFST + ENU_T_MARGIN_BITS,
        ENU_B_MARGIN_BITS = 8,
        ENU_B_MARGIN_MASK = NEW_BITMASK ( ENU_B_MARGIN_OFST, ENU_B_MARGIN_BITS ),

        ENU_TILE_END_BIT = ENU_B_MARGIN_OFST + ENU_B_MARGIN_BITS
    };

    struct ImgTileGetLMargin{
      unsigned int operator()( unsigned int aBitmask ){
        return ( (aBitmask & ENU_L_MARGIN_MASK) >> ENU_L_MARGIN_OFST );
      }
    };

    struct ImgTileGetRMargin {
      unsigned int operator()( unsigned int aBitmask ){
        return ( (aBitmask & ENU_R_MARGIN_MASK) >> ENU_R_MARGIN_OFST );
      }
    };

    struct ImgTileGetTMargin {
      unsigned int operator()( unsigned int aBitmask ){
        return ( (aBitmask & ENU_T_MARGIN_MASK) >> ENU_T_MARGIN_OFST );
      }
    };

    struct ImgTileGetBMargin {
      unsigned int operator()( unsigned int aBitmask ){
        return ( (aBitmask & ENU_B_MARGIN_MASK) >> ENU_B_MARGIN_OFST );
      }
    };

    typedef struct _image_entry_sec{
        unsigned int mBasicAttr;                 //!< Image basic attrbute( colorformat, width, height )
        unsigned int mAttrTile;                  //!< Image tile attribute( left_margin, right_margin, top_margin, bottom_margin )
        unsigned int mDataOffset;                //!< Image raw data's offset(int bytes) in file
    }  Image_Entry_Section;

    typedef BinExtract::FileSection< Image_Entry_Section > ImageEntry;

    typedef struct  _image_raw_data{
        char* mPtrRawData;              //!< allocated from heap
        unsigned int mSzBytes;          //!< total Size of raw data
    } Image_Raw_Data;

    /* ------------------------------------------------------------*/
    /**
     * @Brief ImageRawData section's structure need overwrite read/write
     * because it contain a heap resource need write out
     */
    /* ------------------------------------------------------------*/
    class ImageRawData : public BinExtract::FileSection< Image_Raw_Data > {
        public:
            explicit ImageRawData( unsigned int aSize );
            explicit ImageRawData( const Image_Raw_Data* aClient );

            virtual ~ImageRawData();

            virtual int read( IO::istream* input_stream );

            virtual int write( IO::ostream* output_stream );

            virtual int getSize();
          private:
            bool mNeedFreeRes;
    };

    /* ------------------------------------------------------------*/
    /**
     * @Brief class stand for an Image Binary, which you can get
     *  an image's information by handle. Image's handle is the offset of
     *  image entry in the binary file
     */
    /* ------------------------------------------------------------*/
    class ImageBinFile : BinExtract::AbstractFile{
        public:
            explicit ImageBinFile( const char* pBinFileName );

            unsigned int    getFileSz();

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
            Image_Raw_Data*   getImageData( const ImageEntry* aImageEntry );

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
             * @Brief get attribute for tile image
             *
             * @Param aHandle
             *
             * @return
             */
            /* ------------------------------------------------------------*/
            unsigned int    getImageTile_L_Margin( const int aHandle );

            /* ------------------------------------------------------------*/
            /**
             * @Brief
             *
             * @Param aHandle
             *
             * @return
             */
            /* ------------------------------------------------------------*/
            unsigned int    getImageTile_R_Margin( const int aHandle );

            /* ------------------------------------------------------------*/
            /**
             * @Brief
             *
             * @Param aHandle
             *
             * @return
             */
            /* ------------------------------------------------------------*/
            unsigned int    getImageTile_T_Margin( const int aHandle );

            /* ------------------------------------------------------------*/
            /**
             * @Brief
             *
             * @Param aHandle
             *
             * @return
             */
            /* ------------------------------------------------------------*/
            unsigned int    getImageTile_B_Margin( const int aHandle );

            /* ------------------------------------------------------------*/
            /**
             * @Brief overload read because we want save filesize when input stream setup first time
             *
             * @Param input : the input stream
             *
             * @return actual size read from input stream
             */
            /* ------------------------------------------------------------*/
            //virtual int     read( istream* input = NULL );

            /* ------------------------------------------------------------*/
            /**
             * @Brief overload write because we don't need write for now
             *
             * @Param ouput
             *
             * @return always -1 stand for not support
             */
            /* ------------------------------------------------------------*/
            virtual int     write( IO::ostream* ouput = NULL );

        private:
    };
}

#endif
