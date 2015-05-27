/*
 * Package : mod_dart
 * Author : S. Hamblett <steve.hamblett@linux.com>
 * Date   : 26/05/2015
 * Copyright :  S.Hamblett 2015
 * License : GPL V3, see the LICENSE file for details
 */


#ifndef TEMPLATE_H
#define	TEMPLATE_H

#include <stdio.h>

#include "ctemplate-1.0/ctemplate.h"

typedef  TMPL_varlist tpl_varlist;

#ifdef	__cplusplus
extern "C" {
#endif
    
    tpl_varlist * tpl_addVar( const char *name, const char *value,tpl_varlist * varList);
    
    int tpl_write(const char *filename, const tpl_varlist *varlist, FILE *out);

    void tpl_free(tpl_varlist *varlist);
    
#ifdef	__cplusplus
}
#endif

#endif	/* TEMPLATE_H */

