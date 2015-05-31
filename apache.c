/*
 * Package : mod_dart
 * Author : S. Hamblett <steve.hamblett@linux.com>
 * Date   : 26/05/2015
 * Copyright :  S.Hamblett 2015
 * License : GPL V3, see the LICENSE file for details
 */


#include <http_core.h>
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
    
    /* QUERY_STRING */
    char* queryString = apr_pstrdup(r->pool, r->parsed_uri.query);
    varlist = tpl_addVar("server_query_string", queryString, varlist);
    
    /* DOCUMENT_ROOT */
    const char* documentRoot = ap_document_root(r);
    varlist = tpl_addVar("server_document_root", documentRoot, varlist);
    
    /* HTTP_ACCEPT */
    const char* accept = apr_table_get(r->headers_in, "Accept");
    varlist = tpl_addVar("server_http_accept", accept, varlist);
    
    /* HTTP_ACCEPT_CHARSET */
    const char* acceptCharset = apr_table_get(r->headers_in, "Accept-Charset");
    varlist = tpl_addVar("server_http_accept_charset", acceptCharset, varlist);
    
    /* HTTP_ACCEPT_ENCODING */
    const char* acceptEncoding = apr_table_get(r->headers_in, "Accept-Encoding");
    varlist = tpl_addVar("server_http_accept_encoding", acceptEncoding, varlist);
    
    /* HTTP_ACCEPT_LANGUAGE */
    const char* acceptLanguage = apr_table_get(r->headers_in, "Accept-Language");
    varlist = tpl_addVar("server_http_accept_language", acceptLanguage, varlist);
    
    /* HTTP_CONNECTION */
    const char* acceptConnection = apr_table_get(r->headers_in, "Connection");
    varlist = tpl_addVar("server_http_connection", acceptConnection, varlist);
    
    /* HTTP_HOST */
    const char* httpHost = apr_table_get(r->headers_in, "Host");
    varlist = tpl_addVar("server_http_host", httpHost, varlist);
    
    /* HTTP_REFERER */
    const char* httpReferer = apr_table_get(r->headers_in, "Referer");
    varlist = tpl_addVar("server_http_referer", httpReferer, varlist);
    
    /* HTTP_USER_AGENT */
    const char* httpUserAgent = apr_table_get(r->headers_in, "User-Agent");
    varlist = tpl_addVar("server_http_user_agent", httpUserAgent, varlist);
    
    return varlist;
    
    /* Key Host Value localhostKey Connection Value keep-aliveKey Cache-Control Value max-age=0Key Accept Value text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,;q=0.8Key User-Agent Value Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/43.0.2357.65 Safari/537.36Key DNT Value 1Key Accept-Encoding Value gzip, deflate, sdchKey Accept-Language Value en-GB,en-US;q=0.8,en;q=0.6*/

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

