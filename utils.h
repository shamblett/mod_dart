/*
 * Package : mod_dart
 * Author : S. Hamblett <steve.hamblett@linux.com>
 * Date   : 26/05/2015
 * Copyright :  S.Hamblett 2015
 * License : GPL V3, see the LICENSE file for details
 */

#ifndef UTILS_H
#define	UTILS_H

#include <apr_file_io.h> 
#include <apr_pools.h>

#ifdef	__cplusplus
extern "C" {
#endif

apr_file_t* getTempFile(char* template, apr_pool_t* pool);


#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_H */

