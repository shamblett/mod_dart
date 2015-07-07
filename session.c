/*
 * Package : mod_dart
 * Author : S. Hamblett <steve.hamblett@linux.com>
 * Date   : 26/05/2015
 * Copyright :  S.Hamblett 2015
 * License : GPL V3, see the LICENSE file for details
 */

#include "session.h" 

static int (*ap_session_load_fn) (request_rec * r, session_rec ** z) = NULL;
static int (*ap_session_save_fn) (request_rec * r, session_rec * z) = NULL;
static apr_status_t(*ap_session_get_fn)(request_rec * r, session_rec * z,
        const char *key, const char **value) = NULL;
static apr_status_t(*ap_session_set_fn)(request_rec * r, session_rec * z,
        const char *key, const char *value) = NULL;

int loadSessionModule() {

    int retVal = TRUE;

    if (!ap_session_load_fn || !ap_session_save_fn || !ap_session_get_fn || !ap_session_set_fn) {
        ap_session_load_fn = APR_RETRIEVE_OPTIONAL_FN(ap_session_load);
        ap_session_save_fn = APR_RETRIEVE_OPTIONAL_FN(ap_session_save);
        ap_session_get_fn = APR_RETRIEVE_OPTIONAL_FN(ap_session_get);
        ap_session_set_fn = APR_RETRIEVE_OPTIONAL_FN(ap_session_set);
        if (!ap_session_load_fn || !ap_session_save_fn || !ap_session_get_fn || !ap_session_set_fn) {
            retVal = FALSE;
        }
    }

    return retVal;
}

int hasSession(request_rec* r) {

    session_rec* theSession;

    if (!loadSessionModule()) return -1;

    apr_status_t status = ap_session_load_fn(r, &theSession);
    if (status != APR_SUCCESS) return FALSE;
    return TRUE;
}

int sessionStart(request_rec* r, dartSession* session) {

    if (!loadSessionModule()) return FALSE;

    apr_status_t status = ap_session_load_fn(r, &session->modSession);
    if (status != APR_SUCCESS) return FALSE;
    session->isActive = TRUE;
    return TRUE;
}

int sessionDestroy(request_rec* r, dartSession* session) {   

    if (!loadSessionModule()) return FALSE;

    
    /* Set max age to 1 second */
    session->isActive = FALSE;
    session->modSession->maxage = 1;
    apr_status_t status = ap_session_save_fn(r, session->modSession);
    
    if (status != APR_SUCCESS) return FALSE;
    return TRUE;
}

int sessionSet(request_rec* r, dartSession* session, const char *key, const char *value) {
    
    
    if (!loadSessionModule()) return FALSE;
    
    session->isActive = TRUE;
    apr_status_t status = ap_session_set_fn(r, session->modSession, key, value);
    if (status != APR_SUCCESS) return FALSE;
    return TRUE;
}

int sessionSave(request_rec* r, dartSession* session, int force) {
    
    if (!loadSessionModule()) return FALSE;
    
    session->isActive = TRUE;
    if ( force == TRUE ) session->modSession->dirty = TRUE;
    apr_status_t status = ap_session_save_fn(r, session->modSession);
    if (status != APR_SUCCESS) return FALSE;
    return TRUE;
    
}

