/**
 * @file DoubleLinkList.h
 * @Brief Double Link List data structure define
 * @author Sven Zhang
 * @version 0.01
 * @date 2016-01-27
 */

# ifndef DOUBLELINKLIST_H
# define DOUBLELINKLIST_H
# include "common.h"

CPLUSPLUS_BEGIN

// definition
// DLL --> DoubleLinkList

typedef struct _DLL_NODE{
    struct _DLL_NODE        *ptr_prev;
    struct _DLL_NODE        *ptr_next;

    void                    *ptr_data;
}DLL_NODE, *PTR_DLL_NODE;

typedef bool (*FP_NODE_EQUAL)( const PTR_DLL_NODE, const PTR_DLL_NODE );

typedef struct _DLL{
    PTR_DLL_NODE            ptr_head;
    PTR_DLL_NODE            ptr_tail;

    FP_NODE_EQUAL           fp_node_eq;             //!< the operator to indentify two node equal or not

    unsigned int            total_node_amount;
    unsigned int            node_inuse;
}DLL, *PTR_DLL;

typedef void (*FP_NODE_VISITOR)( void* );

// public procedures
void            DLL_init( PTR_DLL ptr_this );
void            DLL_deinit( PTR_DLL ptr_this );

/* ------------------------------------------------------------*/
/**
 * @Brief insert given node to linklist as the first item
 *
 * @Param ptr_this the DoubleLinkList
 * @Param ptr_node_insert: the Node to insert
 */
/* ------------------------------------------------------------*/
void            DLL_insert( PTR_DLL ptr_this, PTR_DLL_NODE ptr_node_insert );

/* ------------------------------------------------------------*/
/**
 * @Brief remove given Node from linlist
 *
 * @Param ptr_this : the DoubleLinkList
 * @Param ptr_node_remove : the Node to remove
 *
 * @return false means can't find given node
 *         true means operation done
 */
/* ------------------------------------------------------------*/
bool            DLL_remove( PTR_DLL ptr_this, PTR_DLL_NODE ptr_node_remove );

/* ------------------------------------------------------------*/
/**
 * @Brief Find the first node which doesn't include any data from
 * head to tail
 *
 * @Param ptr_this the DoubleLinkList to search
 *
 * @return if found a node doesn't contain data then return the node's address
 *          else return NULL_PTR
 */
/* ------------------------------------------------------------*/
PTR_DLL_NODE    DLL_find_first_free_node( PTR_DLL ptr_this );

/* ------------------------------------------------------------*/
/**
 * @Brief Find the given node in the DLL
 *
 * @Param ptr_this the doublelinklist to search
 * @Param ptr_node_find the node which need be searched
 *
 * @return if given node found return true else return false
 */
/* ------------------------------------------------------------*/
bool            DLL_find_node( const PTR_DLL ptr_this, PTR_DLL_NODE ptr_node_find );

/* ------------------------------------------------------------*/
/**
 * @Brief move give node before the reference node
 *
 * @Param ptr_this
 * @Param ptr_node_move
 * @Param ptr_node_ref
 *
 * @return if the can't find the reference node or the node which need moved, return false
 *          else return true if operation done
 */
/* ------------------------------------------------------------*/
bool            DLL_move_node_before    ( PTR_DLL ptr_this, PTR_DLL_NODE ptr_node_move, PTR_DLL_NODE ptr_node_ref );

/* ------------------------------------------------------------*/
/**
 * @Brief move given node after the reference node
 *  @see DLL_move_node_before
 *
 * @Param ptr_this
 * @Param ptr_node_move
 * @Param ptr_node_ref
 *
 * @return
 */
/* ------------------------------------------------------------*/
bool            DLL_move_node_after     ( PTR_DLL ptr_this, PTR_DLL_NODE ptr_node_move, PTR_DLL_NODE ptr_node_ref );

/* ------------------------------------------------------------*/
/**
 * @Brief move the given node to the beginning of the list
 *
 * @Param ptr_this
 * @Param ptr_node_move
 *
 * @return
 */
/* ------------------------------------------------------------*/
bool            DLL_move_node_as_first  ( PTR_DLL ptr_this, PTR_DLL_NODE ptr_node_move );

/* ------------------------------------------------------------*/
/**
 * @Brief Move the given node to the end of the list
 *
 * @Param ptr_this
 * @Param ptr_node_move
 *
 * @return
 */
/* ------------------------------------------------------------*/
bool            DLL_move_node_as_end    ( PTR_DLL ptr_this, PTR_DLL_NODE ptr_node_move );

/* ------------------------------------------------------------*/
/**
 * @Brief Visit each node with given visitor function pointer
 *
 * @Param ptr_this
 * @Param FP_NODE_VISITOR the clientt need provide a function to access the node's data
 */
/* ------------------------------------------------------------*/
void            DLL_visit_each_node( PTR_DLL ptr_this, FP_NODE_VISITOR );

CPLUSPLUS_END

# endif

