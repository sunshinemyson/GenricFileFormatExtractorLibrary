#include "io_base.hpp"

#include "gtest/gtest.h"

TEST( IO_Stream, ofstream )
{
    IO::ofstream    *ofs = new IO::ofstream( "GoogleTest.bin", IO::io_base::out );

    EXPECT_EQ( 0, ofs->tellg() );
    EXPECT_NE( true, ofs->eof() );

    char byte = 'a';
    unsigned int fsz = 0;
    while( !ofs->eof() )
    {
        ofs->write( &byte, 1 );
        fsz++;
    }

    EXPECT_EQ( fsz, ofs->tellg() );

    ofs->write( &byte, 1 );
    EXPECT_EQ( fsz, ofs->tellg() );

    ofs->seekg(0, IO::io_base::beg);
    EXPECT_EQ( 0, ofs->tellg() );

    ofs->seekg(fsz/2, IO::io_base::cur);
    EXPECT_EQ( fsz/2, ofs->tellg() );

    ofs->seekg( -1, IO::io_base::cur );
    EXPECT_EQ( fsz/2-1, ofs->tellg() );

    ofs->seekg( fsz, IO::io_base::cur );
    EXPECT_TRUE( ofs->eof() );

    ofs->write( &byte, 1 );
    EXPECT_EQ( fsz, ofs->tellg() );

    delete ofs;

    ofs = new IO::ofstream( "GoogleTest.bin", IO::io_base::out );
    EXPECT_EQ(0,ofs->tellg());

    int v = 0;
    while( !ofs->eof() )
    {
        ofs->write( (char*)&v, sizeof(v) );
        v++;
    }
    delete ofs;
}

TEST( IO_Stream, ifstream )
{
    IO::ifstream *ifs = new IO::ifstream( "GoogleTest.bin", IO::io_base::in );

    int read_value = 0;
    int value_writed_by_last_test = 0;
    while( !ifs->eof() )
    {
         ifs->read( (char*)&read_value, sizeof(int) );
         EXPECT_EQ( read_value, value_writed_by_last_test );
         value_writed_by_last_test ++;
    }

    int oldValue = read_value;
    EXPECT_TRUE( ifs->eof() );
    ifs->read( (char*)&read_value, sizeof(int) );
    EXPECT_EQ( oldValue, read_value );
    ifs->read( (char*)&read_value, 0 );
    ifs->read( (char*)&read_value, 0 );

    EXPECT_EQ( ifs->tellg(), value_writed_by_last_test * sizeof(4) );
    ifs->seekg( 0, IO::io_base::beg );
    EXPECT_EQ( 0, ifs->tellg() );
    ifs->seekg( -1, IO::io_base::cur );
    EXPECT_EQ( 0, ifs->tellg() );
    ifs->seekg( 0xFFFF , IO::io_base::cur );
    EXPECT_EQ( value_writed_by_last_test * sizeof(4), ifs->tellg() );

    delete ifs;
}
