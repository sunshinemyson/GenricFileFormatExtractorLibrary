/**
 * @file common.h
 * @Brief common tools for all module
 * @author Sven Zhang
 * @version 0.01
 * @date 2016-01-27
 */
# ifndef COMMON_H
# define COMMON_H

# ifndef CPLUSPLUST_BEGIN
#   ifdef __cplusplus
#       define CPLUSPLUS_BEGIN extern "C"{
#       define CPLUSPLUS_END }
#   endif
# endif

# ifndef CNT_OF_ARRAY
#   define CNT_OF_ARRAY
#   define cnt_of_array( _array ) ( sizeof(_array)/sizeof(_array[0]) )
# endif

# endif

