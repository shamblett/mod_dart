/*
 * Package : mod_dart
 * Author : S. Hamblett <steve.hamblett@linux.com>
 * Date   : 26/05/2015
 * Copyright :  S.Hamblett 2015
 * License : GPL V3, see the LICENSE file for details
 */

/* Purpose :-
 * 
 * This module contains constants for mod_dart
 */

#ifndef CONSTANTS_H
#define	CONSTANTS_H

#ifdef	__cplusplus
extern "C" {
#endif

    /* Version of mod_dart */
    const char* VERSION = "0.1.0";

    /* Buffer sentinel, must match the declaration in the template*/
    const char* SENTINEL = ":-:mod_dart_control:-:";
    
    /* Sentinel length */
    const int SENTINEL_LENGTH = 22;

#ifdef	__cplusplus
}
#endif

#endif	/* CONSTANTS_H */

