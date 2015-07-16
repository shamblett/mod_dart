/*
 * Package : mod_dart
 * Author : S. Hamblett <steve.hamblett@linux.com>
 * Date   : 26/05/2015
 * Copyright :  S.Hamblett 2015
 * License : GPL V3, see the LICENSE file for details
 */

/* Purpose :-
 * 
 * This module wraps the ctemplate library for use by mod_dart. Its
 * possible in the future another templating library may be used, however
 * for now ctemplate is fine, it is a re-entrant library and thus thread safe.
 */


#ifndef TEMPLATE_H
#define	TEMPLATE_H

#include "platform.h"

#include "../ctemplate-1.0/ctemplate.h"

typedef TMPL_varlist tpl_varlist;
typedef TMPL_loop tpl_loop;

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * tpl_addVar
     * 
     * Adds a variable to the template dictionary
     * 
     * @param name - The variables name
     * @param value - The variables value
     * @param varList - The variable list to use
     * @return - An updated or new variable list
     */
    tpl_varlist* tpl_addVar(const char *name, const char *value, tpl_varlist * varList);

    /**
     * tpl_addLoopVar
     * 
     * Add loop variables to a dctionary 
     * @param keyName
     * @param key
     * @param valName
     * @param val
     * @return the variable list
     */
    tpl_varlist* tpl_addLoopVar(const char* keyName, const char* key,
            const char* valName, const char* val);
    /**
     * tpl_addVarList
     * 
     * Adds a dictionary to a template loop
     * 
     * @param loop - the loop
     * @param varlist - the distionary to add
     * @return - the updated loop
     */
    tpl_loop* tpl_addVarList(tpl_loop* loop, tpl_varlist* varlist);

    /**
     * tpl_addLoop
     * 
     * Add a loop to a template dictionary
     * 
     * @param varlist - the dictionary to add to
     * @param name - the name of the loop
     * @param loop - the loop to add
     * @return - the updated dictionary
     */
    tpl_varlist* tpl_addLoop(tpl_varlist *varlist, const char *name, tpl_loop *loop);


    /**
     * tpl_write
     * 
     * Creates an output file from a template file and a variable list
     * 
     * @param filename - The path of the template file
     * @param varlist - The variable list to use
     * @param out - An open file descriptor to write the output file to
     * @return - Status, 0 means success
     */
    int tpl_write(const char *filename, const tpl_varlist *varlist, FILE *out);

    /**
     * tpl_free
     * 
     * Free the storage occupied by a variable list
     * 
     * @param varlist - The variable list to free
     */
    void tpl_free(tpl_varlist *varlist);

#ifdef	__cplusplus
}
#endif

#endif	/* TEMPLATE_H */

