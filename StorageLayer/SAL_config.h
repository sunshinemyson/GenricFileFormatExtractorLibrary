/**
 * @file SAL_config.h
 * @Brief SAL = Storage Abstract Layer
 * @author Sven Zhang
 * @version 0.01
 * @date 2016-01-27
 */

# ifndef SAL_CONFIG_H
# define SAL_CONFIG_H

# include "common.h"

# define LINUX_FILE_BACKEND

# ifdef LINUX_FILE_BACKEND
#   include <fcntl.h>
#   include <unistd.h>
#   include <sys/uio.h>
#   include <sys/types.h>
#   include <sys/stat.h>
# endif



# endif

