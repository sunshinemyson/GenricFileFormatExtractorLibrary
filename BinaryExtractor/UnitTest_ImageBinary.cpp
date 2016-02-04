 /**
 * @file UnitTest_ImageBinary.cpp
 * @Brief Image Binary format definition
 * @author Sven Zhang
 * @version 0.01
 * @date 2016-02-04
 */
#include "ImageBinary.hpp"

#include "gtest/gtest.h"

#include <string.h>
//#include "GIR_handle.hpp"

#   include <fcntl.h>
#   include <unistd.h>
#   include <sys/uio.h>
#   include <sys/types.h>
#   include <sys/stat.h>

class ImageBinaryTest : public ::testing::Test {
protected:
  static void SetUpTestCase() {
    sImgBinFile = new IMG_BIN::ImageBinFile( fname );
    fid = open( fname, O_RDONLY );
  }

  static void TearDownTestCase() {
    delete sImgBinFile;
    sImgBinFile = NULL;
    close( fid );
    fid = 0;
  }

  static IMG_BIN::ImageBinFile* sImgBinFile;
  static int                    fid;
  static const char*            fname;
};

IMG_BIN::ImageBinFile* ImageBinaryTest::sImgBinFile = NULL;
int                     ImageBinaryTest::fid        = 0;
const char*            ImageBinaryTest::fname       = "img.bin";

TEST_F( ImageBinaryTest, browse_rgb )
{
  EXPECT_TRUE( sImgBinFile != NULL );

  struct stat lfstat;
  fstat( fid, &lfstat );

  EXPECT_EQ( sImgBinFile->getFileSz(), lfstat.st_size );

  char* pMemRGB = NULL;
  {
    const char* pTestRgb = "bg_title_bar_calling.RGB";
    struct stat testFileStat = {0};

    stat( pTestRgb, &testFileStat );
    pMemRGB = new char[testFileStat.st_size];

    int testfid = open( pTestRgb, O_RDONLY );

    int sz = read( testfid, (void*)pMemRGB, testFileStat.st_size );

    assert( sz == testFileStat.st_size );
  }

  IMG_BIN::ImageEntry*      firstImageEntry   = NULL;
  IMG_BIN::Image_Raw_Data*  firstImageRawData = NULL;
  int firstImageOffset = sizeof( char ) + sizeof( int );

  firstImageEntry = sImgBinFile->getImageAttr( firstImageOffset );

  EXPECT_TRUE( NULL != firstImageEntry );

  if( NULL != firstImageEntry )
  {
    firstImageRawData = sImgBinFile->getImageData( firstImageEntry   );

    EXPECT_TRUE( 0 == memcmp( firstImageRawData->mPtrRawData, pMemRGB, firstImageRawData->mSzBytes ) );

    EXPECT_EQ( 0, sImgBinFile->getImageWidth( firstImageOffset ) % 4 );

    IMG_BIN::ImageEntry*  visitEachEntry = NULL;
    int idx = 1;
    for( int imgEntryOffset = firstImageOffset + firstImageEntry->getSize(); imgEntryOffset < firstImageEntry->mData.mDataOffset; imgEntryOffset += firstImageEntry->getSize() )
    {
      visitEachEntry = sImgBinFile->getImageAttr( imgEntryOffset );
      if( visitEachEntry )
      {
        EXPECT_EQ( 0, IMG_BIN::ImgGetWidth()(visitEachEntry->mData.mBasicAttr) % 4 ) << "the " << idx << "'rd width not times of 4";
        EXPECT_TRUE( visitEachEntry->mData.mDataOffset < sImgBinFile->getFileSz() );
        delete visitEachEntry;
      }
      idx ++;
    }
  }

  delete firstImageEntry;
  delete firstImageRawData;

  delete[] pMemRGB;
}

TEST_F( ImageBinaryTest, tileAttributes )
{
  int firstImageOffset = sizeof( char ) + sizeof( int );

  EXPECT_EQ( 15, sImgBinFile->getImageTile_L_Margin(firstImageOffset) );
  EXPECT_EQ( 15, sImgBinFile->getImageTile_R_Margin(firstImageOffset) );
  EXPECT_EQ( 0 , sImgBinFile->getImageTile_T_Margin(firstImageOffset) );
  EXPECT_EQ( 0 , sImgBinFile->getImageTile_B_Margin(firstImageOffset) );
}
