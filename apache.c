/*
 * Package : mod_dart
 * Author : S. Hamblett <steve.hamblett@linux.com>
 * Date   : 26/05/2015
 * Copyright :  S.Hamblett 2015
 * License : GPL V3, see the LICENSE file for details
 */


#include <apr_strings.h>

#include "template.h"
#include "utils.h"
#include "error.h"

#include "apache.h"

apr_file_t* buildApacheClass(char* templatePath, char* cachePath, request_rec *r) {
    
    TMPL_varlist* varList;
    apr_file_t* scriptFile;
    apr_status_t status;
    FILE* fp;
    int tmplStatus = -1;
    char scriptFileTemplate[PATH_MAX];
    char scriptFileName[PATH_MAX];
    
    //TODO
    varList = addVar("version", "0.1.0", varList);
    varList = addVar("ip", "123.456.789.000", varList);
    
    /* Create the template file output file, get its name and close it. */
    scriptFileTemplate = apr_pstrcat(r->pool, cachePath, "XXXXXX", 0);
    scriptFile = getTempFile(scriptFileTemplate, r->pool);
    if ( scriptFile == NULL ) return NULL;
    status = apr_file_name_get(scriptFileName, scriptFile);
    status = apr_file_close(scriptFile);
    
    /* Ctemplate needs normal files, not APR stuff so we become 
     non-portable here for now.
     */
    fp = fopen(scriptFileName, "rw");
    tmplStatus = TMPL_write (templatePath, 0, 0, varList, fp, 0);
    if ( tmplStatus ) {
        
        logError("mod_dart - buildApacheClass - TMPL_write failed", r->pool, 0);
        return NULL;
    }
    
    return scriptFile;
    
    
    
}

