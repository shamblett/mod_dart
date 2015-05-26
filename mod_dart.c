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

#include "apache.h"
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
    FILE *fp = NULL;
    FILE *pcacheFile = NULL;
    char output[PATH_MAX];
    char command[PATH_MAX];
    char cacheFile[PATH_MAX];
    char cpcmd[PATH_MAX];
    int status = -1;
    char* templateBuffer = NULL;
    
    if (!r->handler || strcmp(r->handler, "dart")) return (DECLINED);

    /* Get the filename */
    filename = apr_pstrdup(r->pool, r->filename);
    
    /* Invoke the VM */
    strcpy(command, config.pathToExe);
    strcat(command, " ");
    strcat(command, filename);
    strcat(command, " 2>&1");
    fp = popen(command, "r");
    if (fp == NULL) {

        ap_rprintf(r, "<h2> POPEN Fail %s</h2>", command);
        return (DECLINED);
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
