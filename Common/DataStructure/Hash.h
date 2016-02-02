/**
 * @file Hash.h
 * @Brief Hash list to boost search procedure
 * @author Sven Zhang
 * @version 0.01
 * @date 2016-01-27
 */
# ifndef HASH_H
# define HASH_H

# incldue "common.h"

CPLUSPLUS_BEGIN


struct HASH_NODE;

typedef unsigned int HASH_ID;

typedef struct _hash{
    HASH_NODE       *ptr_hash_lists;                //!<
    unsigned int    sz_hash_lists;                  //!<
    unsigned int    max_overhead;                   //!<
}HASH, *PTR_HASH;

void        Hash_init( PTR_HASH ptr_this, unsigned int cap );
void        Hash_deinit( PTR_HASH ptr_this );

HASH_NODE*  Hash_allocate( PTR_HASH ptr_this, HASH_ID hash_id );
HASH_NODE*  Hash_release ( PTR_HASH ptr_this, HASH_ID hash_id );


CPLUSPLUS_END

#endif

