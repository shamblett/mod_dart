/*
 * Package : mod_dart
 * Author : S. Hamblett <steve.hamblett@linux.com>
 * Date   : 26/05/2015
 * Copyright :  S.Hamblett 2015
 * License : GPL V3, see the LICENSE file for details
 */

#include <stdio.h>

#include <httpd.h>
#include <http_config.h>
#include <http_log.h>
#include <http_protocol.h>
#include <apr_hash.h>
#include <ap_config.h>
#include <ap_provider.h>
#include <apr_strings.h>
#include <apr_file_io.h>

#include "apache.h"
#include "utils.h"
#include "error.h"
#include "mod_dart.h"

typedef struct {
    const char *pathToExe;
    const char *pathToCache;
    const char *pathToTemplate;

} md_config;

static md_config config;

const char *md_set_exe_path(cmd_parms *cmd, void *cfg, const char *arg) {
    config.pathToExe = arg;
    return (const char*) NULL;
}

const char *md_set_cache_path(cmd_parms *cmd, void *cfg, const char *arg) {
    config.pathToCache = arg;
    return (const char*) NULL;
}

const char *md_set_template_path(cmd_parms *cmd, void *cfg, const char *arg) {
    config.pathToTemplate = arg;
    return (const char*) NULL;
}

static int md_handler(request_rec *r) {

    char* filename; 
    char command[PATH_MAX];
    char scriptFileTemplate[PATH_MAX];
    char output[PATH_MAX];
    FILE* fp;
    apr_file_t* scriptFile;
    apr_file_t* apacheClassFile;
    apr_status_t status;
    apr_size_t len;
    const char* scriptFileName;
    const char* apacheFileName;
    
    /* Check for a Dart file */
    if (!r->handler || strcmp(r->handler, "dart")) return (DECLINED);
    
    /* Copy the script file contents to a temp file */
    filename = apr_pstrdup(r->pool, r->filename);
    len = sizeof(scriptFileTemplate);
    apr_cpystrn(scriptFileTemplate, config.pathToCache, len);
    apr_pstrcat(r->pool, scriptFileTemplate, "XXXXXX", NULL);
    scriptFile = getTempFile(scriptFileTemplate, r->pool);
    if (scriptFile == NULL ) {
        logError("md_handler - Failed to create script class  file", r->pool, 0);
        return HTTP_INTERNAL_SERVER_ERROR ;
    }
    status = apr_file_name_get(&scriptFileName, scriptFile);
    status = apr_file_copy(filename, scriptFileName, APR_OS_DEFAULT, r->pool);
    if ( status != APR_SUCCESS) {
        logError("md_handler - Failed to create script file copy", r->pool, status);
        return HTTP_INTERNAL_SERVER_ERROR ;
    }	
    
    /* Build the apache class template and append it to the script file */
    apacheClassFile = buildApacheClass(config.pathToTemplate, config.pathToCache, r);
    if (apacheClassFile == NULL ) {
        logError("md_handler - Failed to create apache class  file", r->pool, status);
        return HTTP_INTERNAL_SERVER_ERROR ;
    }
    status = apr_file_name_get(&apacheFileName, apacheClassFile);
    status = apr_file_append(scriptFileName, apacheFileName, APR_OS_DEFAULT, r->pool);
    if ( status != APR_SUCCESS) {
        logError("md_handler - Failed to append apache class  file", r->pool, status);
        return HTTP_INTERNAL_SERVER_ERROR ;
    }
    
    /* Invoke the VM */
    strcpy(command, config.pathToExe);
    strcat(command, " ");
    strcat(command, scriptFileName);
    strcat(command, " 2>&1");
    fp = popen(command, "r");
    if (fp == NULL) {

        logError("md_handler - POPEN Fail ", r->pool, 0);
        return (HTTP_INTERNAL_SERVER_ERROR );
    }
    
    /**
     * We must first set the appropriate content type, followed by our output.
     */
    ap_set_content_type(r, "text/html");
    while (fgets(output, PATH_MAX, fp) != NULL)
        ap_rprintf(r, "%s", output);
   pclose(fp);
   
   /* Lastly, we must tell the server that we took care of this request and everything went fine.
     * We do so by simply returning the value OK to the server.
     */
    return OK;
}

static void md_register_hooks(apr_pool_t *p) {
    ap_hook_handler(md_handler, NULL, NULL, APR_HOOK_MIDDLE);

}

static const command_rec md_directives[] = {
    AP_INIT_TAKE1("DartExePath", md_set_exe_path, NULL, RSRC_CONF, "The path to the Dart executable"), 
    AP_INIT_TAKE1("CachePath", md_set_cache_path, NULL, RSRC_CONF, "The path to the script cache"), 
    AP_INIT_TAKE1("TemplatePath", md_set_template_path, NULL, RSRC_CONF, "The path to the script template"),
    {NULL}
};


/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA dart_module = {
    STANDARD20_MODULE_STUFF,
    NULL, /* Per-directory configuration handler */
    NULL, /* Merge handler for per-directory configurations */
    NULL, /* Per-server configuration handler */
    NULL, /* Merge handler for per-server configurations */
    md_directives, /* Any directives we may have for httpd */
    md_register_hooks /* Our hook registering function */
};
