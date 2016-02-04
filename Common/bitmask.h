/*****************************************************************************/
/*!
* \file bitmask.h
*
* \brief <b>  </b>
*
* \details
*
* \author  Sven Chang
*
* \date 2015 11 23
*
* \bug see Release Note
*
* \warning Copyright 2015 by Garmin Ltd. or its subsidiaries.
*
******************************************************************************/
#ifndef BITMASK_H
#define BITMASK_H
/*****************************************************************************
 * GENERAL INCLUDES
*****************************************************************************/
#ifndef IO_UTL_H
    //#include "IO_Utl.h"
    #include "common.h"
#endif

CPLUSPLUS_BEGIN
/*****************************************************************************
 * LITERAL CONSTANTS
*****************************************************************************/
/*****************************************************************************
 * TYPES
*****************************************************************************/
typedef UW BITMASK32;
static const int BITMASK_LEN  = (8*sizeof(BITMASK32));

/*****************************************************************************
 * PROCEDURES
*****************************************************************************/
// !!!NOTE: Caller should guarantee parameter valid

#ifndef NEW_BITMASK
// usage BITMASK32 bitmask = NEW_BITMASK( 0, 4 );   // 0xF
    #define NEW_BITMASK( _OFFSET, _LEN ) ( ( ( 0xFFFFFFFFu >> (_OFFSET) ) << ( BITMASK_LEN - (_LEN) ) ) >> ( BITMASK_LEN - (_LEN) - (_OFFSET) ) )
#endif

#ifndef GET_BIT
// usage BITMASK32 bit = GET_BIT( oldMask, 23 )
    #define GET_BIT( _BM, _OFFSET ) (((BITMASK32) _BM) >> (_OFFSET) & 0x1u)
#endif

#ifndef GET_BITS
// usage BITMASK32 bits = GET_BIT( oldMask, 0, <=32 )
    #define GET_BITS( _BM, _OFFSET, _LEN ) NEW_BITMASK( _OFFSET, _LEN ) & (_BM)
#endif

#ifndef SET_BIT
    #define SET_BIT( _BM, _OFFSET ) ( (_BM) |= 0x1u << (_OFFSET) )
#endif

#ifndef SET_BITS
    #define SET_BITS( _BM, _OFFSET, _LEN ) ( (_BM) |= NEW_BITMASK(_OFFSET, _LEN) )
#endif

#ifndef CLR_BITS
    #define CLR_BITS( _BM, _OFFSET, _LEN ) (_BM) &= ~( NEW_BITMASK(_OFFSET, _LEN) )
#endif

#ifndef CLR_BIT
    #define CLR_BIT( _BM, _OFFSET )  (_BM)  &= ~(0x01u<<(_OFFSET))
#endif

#ifndef IDX_MSB
    #define IDX_MSB( _BM )
#endif

#ifndef IDX_LSB
    #define IDX_LSB( _BM )
#endif

#ifndef BIT_CNT
    #define BIT_CNT( _BM, _V )
#endif

#ifndef BIT_CNT_ONE
    #define BIT_CNT_ONE( _BM ) BIT_CNT( _BM, 1 )
#endif

#ifndef BIT_CNT_ZERO
    #define BIT_CNT_ZERO( _BM ) BIT_CNT( _BM, 0 )
#endif

#ifndef TRAIT_BITS
    #define TRAIT_BITS( _MASKED_VALUE, _MASK, _OFST ) ( ( _MASKED_VALUE ) & ( _MASK )) >> _OFST )
#endif
CPLUSPLUS_END

#endif
