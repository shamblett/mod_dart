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
#include "constants.h"

#include "apache.h"

apr_file_t* buildApacheClass(const char* templatePath, const char* cachePath, request_rec *r) {
    
    TMPL_varlist* varList = NULL;
    apr_file_t* scriptFile;
    apr_status_t status;
    FILE* fp;
    int tmplStatus = -1;
    char scriptFileTemplate[PATH_MAX];
    apr_size_t len;
    const char* scriptFileName;
   
    /* Build the apache environment */
    
    /* Version */
    varList = addVar("version", VERSION, NULL);
    
    //TODO
    varList = addVar("ip", "123.456.789.000", varList);
    
    /* Create the template file output file, get its name and close it. */
    len = sizeof(scriptFileTemplate);
    apr_cpystrn(scriptFileTemplate, cachePath, len);
    apr_pstrcat(r->pool, scriptFileTemplate, "XXXXXX", NULL);
    scriptFile = getTempFile(scriptFileTemplate, r->pool);
    if ( scriptFile == NULL ) return NULL;
    status = apr_file_name_get(&scriptFileName, scriptFile);
    status = apr_file_close(scriptFile);
    if ( status != APR_SUCCESS) {
        logError("buildApacheClass - Failed to create apache template file", r->pool, status);
        return NULL;
    }
    
    /* Ctemplate needs normal files, not APR stuff so we become 
     non-portable here for now.
     */
    fp = fopen(scriptFileName, "a");
    tmplStatus = TMPL_write (templatePath, 0, 0, varList, fp, 0);
    if ( tmplStatus ) {
        
        logError("mod_dart - buildApacheClass - TMPL_write failed", r->pool, 0);
        return NULL;
    }
    fclose(fp);
    
    return scriptFile;
    
}

