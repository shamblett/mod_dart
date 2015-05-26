/*
 * Package : mod_dart
 * Author : S. Hamblett <steve.hamblett@linux.com>
 * Date   : 26/05/2015
 * Copyright :  S.Hamblett 2015
 * License : GPL V3, see the LICENSE file for details
 */


#ifndef TEMPLATE_H
#define	TEMPLATE_H

#include "ctemplate-1.0/ctemplate.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
    TMPL_varlist * addVar( const char *name, const char *value, TMPL_varlist * varList);
    

#ifdef	__cplusplus
}
#endif

#endif	/* TEMPLATE_H */

