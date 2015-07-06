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

    /* Our session record */
    typedef struct {
        
        session_rec* modSession;
        int isActive;
        
    } dartSession;
    
    /**
     * sessionStart
     * 
     * Start a session, only use this if you want to do this explicitly,
     * this will either return a new session if one does not exist or an 
     * existing one.
     * 
     * @param r - the request record
     * @param session - the returned session
     * 
     * @return false indicates the session start failed
     */
    int sessionStart(request_rec* r, dartSession* session);
    
    /**
     * sessionDestroy
     * 
     * Destroys a session created by sessionStart
     * 
     * @param r - the request record
     * @param session - the session to destroy
     * 
     * @return false indicates the session destroy failed
     */
    int sessionDestroy(request_rec* r, dartSession* session);
    
    /**
     * hasSession
     * 
     * Indicates if a session is active, to get a session use
     * session start
     * 
     * @param r - the request record
     * 
     * @return  true indicates a session is active.
     */
    int hasSession(request_rec* r);


#ifdef	__cplusplus
}
#endif

#endif	/* SESSION_H */

