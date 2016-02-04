/**
 * @file ImageBinary.cpp
 * @Brief Image Binary format implementation
 * @author Sven Zhang
 * @version 0.01
 * @date 2016-02-04
 */

#include "ImageBinary.hpp"

#include "common.h"

namespace IMG_BIN
{
  // ImageRawData implementation
  ImageRawData::ImageRawData( unsigned int aSize )
    : mNeedFreeRes( true )
  {
    assert( 0 != aSize );

    mData.mPtrRawData = new char[aSize];
    assert( NULL != mData.mPtrRawData );

    mData.mSzBytes = aSize;
  }

  ImageRawData::ImageRawData( const Image_Raw_Data* aClient )
    : mNeedFreeRes( false )
  {
    assert( 0 != aClient );
    mData.mPtrRawData = aClient->mPtrRawData;
    mData.mSzBytes    = aClient->mSzBytes;
  }

  ImageRawData::~ImageRawData()
  {
    if( mNeedFreeRes )
    {
      delete[] mData.mPtrRawData;
      mData.mPtrRawData = NULL;
    }
  }

  int ImageRawData::read( IO::istream* input_stream )
  {
    int readBytes = mData.mSzBytes;

    assert( NULL != input_stream );
    input_stream->read( (char*)mData.mPtrRawData, readBytes );

    return readBytes;
  }

  int ImageRawData::write( IO::ostream* ouput_stream )
  {
    return -1;
  }

  int ImageRawData::getSize()
  {
    return mData.mSzBytes;
  }

  // ImageBinFile implementation
  ImageBinFile::ImageBinFile( const char* pBinFileName )
    : BinExtract::AbstractFile()
  {
      mInputStream = new IO::ifstream( pBinFileName, IO::io_base::in );
  }

  unsigned int ImageBinFile::getFileSz()
  {
    EndianSection   endian(0);
    FileSizeSection fsz(0x00000000U);
    int             curPos;
    int             bytes;

    curPos = rseekTo( 0, BinExtract::AbstractFile::SEEK_F_START );
    (*this)<<(&endian)<<(&fsz);
    bytes = read();

    assert( bytes == endian.getSize() + fsz.getSize() );

    // seek back to original position
    rseekTo( curPos, BinExtract::AbstractFile::SEEK_F_START );

    return fsz.mData;
  }

  Image_Raw_Data* ImageBinFile::getImageData( const ImageEntry* aImageEntry )
  {
    Image_Raw_Data*     PtrRawData;
    int                 curPos;
    int                 rawDataOfst = aImageEntry->mData.mDataOffset;
    int                 imgHeight   = ImgGetWidth()( aImageEntry->mData.mBasicAttr );
    int                 imgWidth    = ImgGetHeight()( aImageEntry->mData.mBasicAttr );
    int                 imgSz       = imgHeight * imgWidth * ImageSize<RGB888>::PixelSize;

    assert( NULL != aImageEntry );

    //printf(" DEBUG : Imagesize = %d(%d*%d) read from offst = %x\n", imgSz, imgWidth, imgHeight, rawDataOfst );

    curPos = rseekTo( rawDataOfst, BinExtract::AbstractFile::SEEK_F_START );
    PtrRawData = new Image_Raw_Data;
    PtrRawData->mPtrRawData = new char[imgSz];
    {
      ImageRawData        RawDataSection( PtrRawData );
      int                 readBytes;
      (*this) << &RawDataSection;
      readBytes = read();
      //printf(" DEBUG : readBytes = %d\n", readBytes );
    }

    rseekTo( curPos, BinExtract::AbstractFile::SEEK_F_START );

    return PtrRawData;
  }

  ImageEntry* ImageBinFile::getImageAttr( const int aImgHandle )
  {
    ImageEntry* imgEntry;
    int         curPos;
    int         readBytes;

    imgEntry = NULL;
    curPos = rseekTo( aImgHandle, BinExtract::AbstractFile::SEEK_F_START );

    imgEntry = new ImageEntry();
    (*this) << imgEntry;
    readBytes = read();
    //printf(" DEBUG : readBytes = %d\n", readBytes );

    rseekTo( curPos, BinExtract::AbstractFile::SEEK_F_START );

    return imgEntry;
  }

  unsigned int ImageBinFile::getImageWidth( const int aImgHandle )
  {
    unsigned int w;
    ImageEntry* imgEntry = getImageAttr( aImgHandle );

    w = ImgGetWidth()( imgEntry->mData.mBasicAttr );

    delete imgEntry;
    return w;
  }

  unsigned int ImageBinFile::getImageHeight( const int aImgHandle )
  {
    unsigned int h;

    ImageEntry *imgEntry = getImageAttr( aImgHandle );

    h = ImgGetHeight()( imgEntry->mData.mBasicAttr );

    delete imgEntry;
    return  0;
  }

  unsigned int ImageBinFile::getImageTile_L_Margin( const int aImgHandle )
  {
    unsigned int lMargin = 0;
    ImageEntry *imgEntry = getImageAttr( aImgHandle );

    lMargin = ImgTileGetLMargin()( imgEntry->mData.mAttrTile );

    delete imgEntry;
    return lMargin;
  }

  unsigned int ImageBinFile::getImageTile_R_Margin( const int aImgHandle )
  {
    unsigned int rMargin = 0;
    ImageEntry *imgEntry = getImageAttr( aImgHandle );

    rMargin = ImgTileGetRMargin()( imgEntry->mData.mAttrTile );

    delete imgEntry;
    return rMargin;
  }

  unsigned int ImageBinFile::getImageTile_T_Margin( const int aImgHandle )
  {
    unsigned int tMargin = 0;
    ImageEntry *imgEntry = getImageAttr( aImgHandle );

    tMargin = ImgTileGetTMargin()( imgEntry->mData.mAttrTile );

    delete imgEntry;
    return tMargin;
  }

  unsigned int ImageBinFile::getImageTile_B_Margin( const int aImgHandle )
  {
    unsigned int bMargin = 0;
    ImageEntry *imgEntry = getImageAttr( aImgHandle );

    bMargin = ImgTileGetBMargin()( imgEntry->mData.mAttrTile );

    delete imgEntry;
    return bMargin;
  }

  // For ImageBinary, we didn't need write API, so always return -1
  /*virtual*/ int ImageBinFile::write( IO::ostream* output )
  {
    return -1;
  }

}
