/*
 * Package : mod_dart
 * Author : S. Hamblett <steve.hamblett@linux.com>
 * Date   : 26/05/2015
 * Copyright :  S.Hamblett 2015
 * License : GPL V3, see the LICENSE file for details
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

