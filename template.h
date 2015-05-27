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

#include <stdio.h>

#include "ctemplate-1.0/ctemplate.h"

typedef TMPL_varlist tpl_varlist;

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
    tpl_varlist * tpl_addVar(const char *name, const char *value, tpl_varlist * varList);

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

