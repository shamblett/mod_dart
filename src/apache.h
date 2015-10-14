/*
 * Package : mod_dart
 * Author : S. Hamblett <steve.hamblett@linux.com>
 * Date   : 26/05/2015
 * Copyright :  S.Hamblett 2015
 * License : GPL V3, see the LICENSE file for details
 */

/* Purpose :-
 * 
 * This module contains the functions needed to query the Apache environment
 * and build the Apache class from the supplied template file.
 */

#ifndef APACHE_H
#define	APACHE_H

#include "platform.h"

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * buildApacheClass
     * 
     * Builds the Apache class from the apache environment.
     * 
     * @param templatePath - The path to the Apache class template
     * @param cachePath - The path to the cache directory
     * @param sessionsOn - indicates if sessions are on in the vhost
     * @param r - The apache request structure
     * @return - An APR files descriptor of the built Apache class script
     */
    apr_file_t* buildApacheClass(const char* templatePath, const char* cachePath, 
                                 int sessionsOn, request_rec *r);

    /**
     * parseBuffer
     * 
     * Parse the output from the VM, apply the control commands
     * and return the real output;
     * 
     * @param input - The VM output
     * @param sessionsOn - indicates if sessions are on in the vhost
     * @param r - the current request record
     * @return - the buffer to write to apache
     */
    char* parseBuffer(char* input, int sessionsOn, request_rec* r);

#ifdef	__cplusplus
}
#endif

#endif	/* APACHE_H */

