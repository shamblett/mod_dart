/*
 * Package : mod_dart
 * Author : S. Hamblett <steve.hamblett@linux.com>
 * Date   : 26/05/2015
 * Copyright :  S.Hamblett 2015
 * License : GPL V3, see the LICENSE file for details
 */

/* Purpose :-
 * 
 * This module contains the interface to session handling for
 * mod_dart
 */

#include <mod_session.h>

#ifndef SESSION_H
#define	SESSION_H

#ifdef	__cplusplus
extern "C" {
#endif

    int sessionStart();
    
    int sessionDestroy();


#ifdef	__cplusplus
}
#endif

#endif	/* SESSION_H */

