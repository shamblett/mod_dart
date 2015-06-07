/*
 * Package : mod_dart
 * Author : S. Hamblett <steve.hamblett@linux.com>
 * Date   : 26/05/2015
 * Copyright :  S.Hamblett 2015
 * License : GPL V3, see the LICENSE file for details
 */

/* Purpose :-
 * 
 * This module provides general purpose utility functions for use throughout mod_dart.
 */

#ifndef UTILS_H
#define	UTILS_H

#include "platform.h"

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * getTempFile
     * Safely creates and returns a file handle toto a temprary file
     * 
     * @param template - The template to use, including the file path,
     * see mkstmp
     * @param pool - The pool to use 
     * @return - An APR file type descriptor for the created file
     */
    apr_file_t* getTempFile(char* template, apr_pool_t* pool);

    /**
     * trimWhiteSpace
     * 
     * Trims leading and trailing whitespace from a string
     * @param s - the string
     * @return - the stripped string
     */
    char* trimWhiteSpace(char *s);
        
#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_H */

