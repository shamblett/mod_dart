/*
 * Package : mod_dart
 * Author : S. Hamblett <steve.hamblett@linux.com>
 * Date   : 26/05/2015
 * Copyright :  S.Hamblett 2015
 * License : GPL V3, see the LICENSE file for details
 */

/* Purpose :-
 * 
 * This module contains error output procedures for use  throughout
 * mod_dart.
 */

#ifndef ERROR_H
#define	ERROR_H

#include <apr_pools.h>

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * logError
     * 
     * Logs an error line in the apache error log
     * 
     * @param message - The error message
     * @param pool - The pool to use
     * @param status - An APR call satus code, or 0
     */
    void logError(char* message, apr_pool_t* pool, apr_status_t status);


#ifdef	__cplusplus
}
#endif

#endif	/* ERROR_H */

