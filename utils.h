/* 
 * File:   utils.h
 * Author: steve
 *
 * Created on 26 May 2015, 09:35
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

