/* 
 * File:   error.h
 * Author: steve
 *
 * Created on 26 May 2015, 09:54
 */

#ifndef ERROR_H
#define	ERROR_H

#include <apr_pools.h>

#ifdef	__cplusplus
extern "C" {
#endif

    void logError(char* message, apr_pool_t* pool, apr_status_t status);


#ifdef	__cplusplus
}
#endif

#endif	/* ERROR_H */

