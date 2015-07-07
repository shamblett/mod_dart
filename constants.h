/*
 * Package : mod_dart
 * Author : S. Hamblett <steve.hamblett@linux.com>
 * Date   : 26/05/2015
 * Copyright :  S.Hamblett 2015
 * License : GPL V3, see the LICENSE file for details
 */

/* Purpose :-
 * 
 * This module contains constants and supporting functions for mod_dart
 */

#ifndef CONSTANTS_H
#define	CONSTANTS_H

#ifdef	__cplusplus
extern "C" {
#endif

    /* Version of mod_dart */
    const char* VERSION = "0.2.0";

    /* Buffer sentinel, must match the declaration in the template*/
    const char* SENTINEL = ":-:mod_dart_control:-:";

    /* Sentinel length */
    const int SENTINEL_LENGTH = 22;

    /* Control buffer objects */
    const char* CB_HEADERS = "Headers";
    const char* CB_SESSION = "Session";
    const char* CB_SESSION_ACTIVE = "Session_Active";
    const char* CB_END = "End";

    enum {
        CB_INT_HEADERS = 1,
        CB_INT_SESSION = 2,
        CB_INT_SESSION_ACTIVE = 3,
        CB_INT_END = 10
    };

    /* Headers */
    const char* H_CONTENT_TYPE = "Content-Type";

    enum {
        H_INT_CONTENT_TYPE = 1,
        H_INT_NORMAL = 100
    };

    /* Control buffer switch tables and definitions */
    typedef struct _switchTableType {
        int switchValue;
        char* switchText;
    } switchTableType;

    const switchTableType cb_switchTable[4] = {
        {1, "Headers"},
        {2, "Session"},
        {3, "Session_Active"},
        {10, "End"}
    };

    const switchTableType h_switchTable[1] = {
        {H_INT_CONTENT_TYPE, "Content-Type"}
    };

    const int getCBSwitchInt(const char* text) {

        int index;

        for (index = 0; index <= sizeof (cb_switchTable); index++) {

            if (!strcmp(cb_switchTable[index].switchText, text)) {
                return cb_switchTable[index].switchValue;
            }
        }

        return CB_INT_END;

    };

    const int getHSwitchInt(const char* text) {

        int index;

        for (index = 0; index <= sizeof (cb_switchTable); index++) {

            if (!strcmp(h_switchTable[index].switchText, text)) {
                return h_switchTable[index].switchValue;
            }
        }

        return H_INT_NORMAL;

    }

#ifdef	__cplusplus
}
#endif

#endif	/* CONSTANTS_H */

