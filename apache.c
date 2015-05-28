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

//TODO leave for now, used to get table contents
/*static int printitem(void *rec, const char *key, const char *value)
{
    request_rec *r = rec;
    ap_rprintf(r,"Key %s Value %s", key, value);
    return 1;
}*/

tpl_varlist* getServerGlobal(request_rec* r, tpl_varlist* varlist) {

    apr_status_t status;
    char ctime[APR_CTIME_LEN];
    char* addr;

    /* SELF */
    char* self = apr_pstrdup(r->pool, r->filename);
    varlist = tpl_addVar("server_self", self, varlist);

    /* SERVER_ADDR */
    status = apr_sockaddr_ip_get(&addr, r->connection->local_addr);
    if (status != APR_SUCCESS) {

        varlist = tpl_addVar("server_addr", "Unable To Obtain", varlist);
    
    } else {
        
        varlist = tpl_addVar("server_addr", addr, varlist);

    }

    /* SERVER_NAME */
    char* hostname = apr_pstrdup(r->pool, r->server->server_hostname);
    varlist = tpl_addVar("server_name", hostname, varlist);
     
    /* SERVER_SOFTWARE */
    const char* serverSoftware = ap_get_server_description();
    varlist = tpl_addVar("server_software", serverSoftware, varlist);
   
    /* SERVER_PROTOCOL */
    char* protocol = apr_pstrdup(r->pool, r->protocol);
    varlist = tpl_addVar("server_protocol", protocol, varlist);
    
    /* REQUEST_METHOD */
    char* method = apr_pstrdup(r->pool, r->method);
    varlist = tpl_addVar("server_request_method", method, varlist);
    
    /* REQUEST_TIME */
    status = apr_ctime(ctime, r->request_time);
    varlist = tpl_addVar("server_request_time", ctime, varlist);
    
    return varlist;

}

apr_file_t* buildApacheClass(const char* templatePath, const char* cachePath, request_rec* r) {

    tpl_varlist* varList = NULL;
    apr_file_t* scriptFile;
    apr_status_t status;
    FILE* fp;
    int tmplStatus = -1;
    char scriptFileTemplate[PATH_MAX];
    char* scriptFileTemplatePath;
    apr_size_t len;
    const char* scriptFileName;

    /* Build the apache environment */

    /* Version */
    varList = tpl_addVar("version", VERSION, NULL);

    /* SERVER super global */
    varList = getServerGlobal(r, varList);

    /* Create the template file output file, get its name and close it. */
    len = sizeof (scriptFileTemplate);
    apr_cpystrn(scriptFileTemplate, cachePath, len);
    scriptFileTemplatePath = apr_pstrcat(r->pool, scriptFileTemplate, "XXXXXX", NULL);
    scriptFile = getTempFile(scriptFileTemplatePath, r->pool);
    if (scriptFile == NULL) return NULL;
    status = apr_file_name_get(&scriptFileName, scriptFile);
    status = apr_file_close(scriptFile);
    if (status != APR_SUCCESS) {
        logError("buildApacheClass - Failed to create apache template file", r->pool, status);
        return NULL;
    }

    /* Template needs normal files, not APR types, create the template file  */
    fp = fopen(scriptFileName, "a");
    tmplStatus = tpl_write(templatePath, varList, fp);
    if (tmplStatus) {

        logError("mod_dart - buildApacheClass - TMPL_write failed", r->pool, 0);
        return NULL;
    }
    fclose(fp);

    /* Free the var list */
    tpl_free(varList);

    return scriptFile;

}

