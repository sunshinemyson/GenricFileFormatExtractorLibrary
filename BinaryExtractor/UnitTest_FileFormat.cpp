/**
 * @file UnitTest_FileFormat.cpp
 * @Brief Unit test for file format
 * @author Sven Zhang
 * @version 0.01
 * @date 2016-02-03
 */

#include "FileFormat.hpp"
#include "io_base.hpp"

#include "gtest/gtest.h"

typedef BinExtract::FileSection<int> IntSection;
typedef BinExtract::FileSection<char> CharSection;

TEST( BinFileFormat, filesection )
{

   IntSection intSection ;

   int op_rslt              = 0;
   int data_writen          = 0xF0F00F0F;
   int  bytesWriten         = 0;
   int  bytesRead           = 0;

   intSection.mData = data_writen;

   IO::ostream *ofs = new IO::ofstream( "GoogleTest.bin", IO::io_base::out );

   while( !ofs->eof() && -1 != op_rslt )
   {
        op_rslt = intSection.write( ofs );
        bytesWriten += sizeof(int);
   }

   EXPECT_EQ( bytesWriten, ofs->tellp() ) << "af_writeter write data to output until meet eof() condition, in our test we need ensure the filesize of binary used to test is times of 4 ";
   op_rslt = intSection.write( ofs );
   EXPECT_EQ( bytesWriten, ofs->tellp() ) << "any write operation will not taken af_writeter eof() meet, until you seek the file to a valid position";
   ofs->seekp( -4, IO::io_base::cur );
   EXPECT_EQ( bytesWriten-4, ofs->tellp() );
   op_rslt = intSection.write( ofs );
   EXPECT_EQ( 4, op_rslt );
   EXPECT_EQ( bytesWriten, ofs->tellp() );

   delete ofs;

   IO::istream *ifs = new IO::ifstream( "GoogleTest.bin", IO::io_base::in );
   intSection.mData = 0;
   op_rslt = 0;
   EXPECT_EQ( 0, ifs->tellg() );

   while( !ifs->eof() && -1 != op_rslt )
   {
       op_rslt = intSection.read( ifs );
       EXPECT_EQ(4, op_rslt);
       EXPECT_EQ( data_writen, intSection.mData ) << "file content should keep same as writen";
       bytesRead += sizeof(int);
   }

   delete ifs;

}

TEST( BinFileFormat, AbstractBinFile )
{
    IO::ostream *ofs = new IO::ofstream( "GoogleTest.bin", IO::io_base::out );
    IO::istream *ifs = new IO::ifstream( "GoogleTest.bin", IO::io_base::in  );
    int         op_rslt = -1;
    int         section_cnt = 0;

    {
        IntSection intSec( 0xFF00FF00 );
        CharSection chSec0( 'a' );
        CharSection chSec1( 'b' );
        CharSection chSec2( 'c' );
        CharSection chSec3( 'd' );

        BinExtract::AbstractFile af_write;
        af_write << &intSec << &chSec0 << &chSec1 << &chSec2 << & chSec3 ;
        op_rslt = af_write.write( ofs );
        section_cnt ++;
        EXPECT_EQ( op_rslt, sizeof(IntSection::value_type) + 4*sizeof(CharSection::value_type) );

        while( !ofs->eof() && -1 != op_rslt )
        {
            af_write << &intSec << &chSec0 << &chSec1 << &chSec2 << & chSec3 ;
            op_rslt = af_write.write();
            section_cnt ++;
        }

        EXPECT_EQ( -1, af_write.rseekTo( 1 ) );
    }// Until now, the ofs release by af_write in its dis-constructor

    {
        BinExtract::AbstractFile af_read;
        IntSection  intSec( 0 );
        CharSection chSec0( '\0' );
        CharSection chSec1( '\0' );
        CharSection chSec2( '\0' );
        CharSection chSec3( '\0' );

        IntSection intSecOrg( 0xFF00FF00 );
        CharSection chSec0Org( 'a' );
        CharSection chSec1Org( 'b' );
        CharSection chSec2Org( 'c' );
        CharSection chSec3Org( 'd' );


        af_read << &intSec << &chSec0 << &chSec1 << &chSec2 << &chSec3 ;
        op_rslt = af_read.read( ifs );
        section_cnt --;

        EXPECT_EQ( intSec.mData, intSecOrg.mData );
        EXPECT_EQ( chSec0.mData, chSec0Org.mData );
        EXPECT_EQ( chSec1.mData, chSec1Org.mData );
        EXPECT_EQ( chSec2.mData, chSec2Org.mData );
        EXPECT_EQ( chSec3.mData, chSec3Org.mData );

        while( !ifs->eof() && -1 != op_rslt )
        {

            af_read << &intSec << &chSec0 << &chSec1 << &chSec2 << &chSec3 ;
            section_cnt --;
            op_rslt = af_read.read( ifs );

            EXPECT_EQ( intSec.mData, intSecOrg.mData );
            EXPECT_EQ( chSec0.mData, chSec0Org.mData );
            EXPECT_EQ( chSec1.mData, chSec1Org.mData );
            EXPECT_EQ( chSec2.mData, chSec2Org.mData );
            EXPECT_EQ( chSec3.mData, chSec3Org.mData );
        }
        EXPECT_EQ( 0, section_cnt );

        ifs->seekg( 0, IO::io_base::beg );

        while( !ifs->eof() && -1 != op_rslt )
        {

            af_read << &chSec0 << &chSec1 << &chSec2 << &chSec3 ;
            af_read.rseekTo( af_read.rseekTo(0) + intSecOrg.getSize() );
            op_rslt = af_read.read();

            EXPECT_EQ( chSec0.mData, chSec0Org.mData );
            EXPECT_EQ( chSec1.mData, chSec1Org.mData );
            EXPECT_EQ( chSec2.mData, chSec2Org.mData );
            EXPECT_EQ( chSec3.mData, chSec3Org.mData );
        }

        EXPECT_EQ( -1, af_read.wseekTo( 1 ) );
    }// until now, the ifs release by af_read in its dis-constructor


}

