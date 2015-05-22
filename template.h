/* 
 * File:   template.h
 * Author: steve
 *
 * Created on 21 May 2015, 09:44
 */

#ifndef TEMPLATE_H
#define	TEMPLATE_H

#include <stdio.h>
#include "ctemplate-1.0/ctemplate.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
    char* getApacheClass();
    
    void setTemplatePath(const char*path);
    
    void setCachePath(const char* path);
    
    void addVar( const char *name, const char *value);
    

#ifdef	__cplusplus
}
#endif

#endif	/* TEMPLATE_H */

