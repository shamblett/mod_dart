/* 
 * File:   template.h
 * Author: steve
 *
 * Created on 21 May 2015, 09:44
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

