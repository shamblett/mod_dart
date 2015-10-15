/*
 * Package : mod_dart
 * Author : S. Hamblett <steve.hamblett@linux.com>
 * Date   : 26/05/2015
 * Copyright :  S.Hamblett 2015
 * License : GPL V3, see the LICENSE file for details
 */


/* Purpose :-
 * 
 * This module is the apache module interface for mod_dart, it gets module configuration,
 * builds the Apache class, invokes the Dart VM and collects and parses its output.
 */

#include "platform.h"
#include "apache.h"
#include "utils.h"
#include "error.h"
#include "../popen-noshell/popen_noshell.h"
#include "mod_dart.h"

/*
 Module configuration 
 */
typedef struct {
    const char *pathToExe;
    const char *pathToCache;
    const char *pathToTemplate;
    const char *packageRoot;
    int packageRootSet;
    int sessionsOn;

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

const char *md_set_package_root(cmd_parms *cmd, void *cfg, const char *arg) {
    config.packageRoot = arg;
    config.packageRootSet = 1;
    return (const char*) NULL;
}

const char *md_set_session(cmd_parms *cmd, void *cfg, const char *arg) {
    if (((strcmp(arg, "On")) || (strcmp(arg, "on"))) == 0)
        config.sessionsOn = 1;

    return (const char*) NULL;
}

/*
 The apache handler 
 */
static int md_handler(request_rec *r) {

    char* filename;
    char scriptFileTemplate[PATH_MAX];
    char* scriptFileTemplatePath;
    char output[PATH_MAX];
    FILE* fp;
    apr_file_t* scriptFile;
    apr_file_t* apacheClassFile;
    apr_status_t status;
    apr_size_t len;
    const char* scriptFileName;
    const char* apacheFileName;
    char errorBuff[1000];
    const char* packageRoot;
    struct popen_noshell_pass_to_pclose pclose_arg;
    const char *arg2 = NULL;

    /* Check for a Dart file */
    if (!r->handler || strcmp(r->handler, "dart")) return (DECLINED);

    /* Copy the script file contents to a temp file */
    filename = apr_pstrdup(r->pool, r->filename);
    len = sizeof (scriptFileTemplate);
    apr_cpystrn(scriptFileTemplate, config.pathToCache, len);
    scriptFileTemplatePath = apr_pstrcat(r->pool, scriptFileTemplate, "XXXXXX", NULL);
    scriptFile = getTempFile(scriptFileTemplatePath, r->pool);
    if (scriptFile == NULL) {
        sprintf(errorBuff, "md_handler - Failed to create script class file > %s", scriptFileTemplate);
        logError(errorBuff, r->pool, 0);
        return HTTP_INTERNAL_SERVER_ERROR;
    }

    status = apr_file_name_get(&scriptFileName, scriptFile);
    status = apr_file_copy(filename, scriptFileName, APR_OS_DEFAULT, r->pool);
    if (status != APR_SUCCESS) {
        logError("md_handler - Failed to create script file copy", r->pool, status);
        return HTTP_INTERNAL_SERVER_ERROR;
    }

    /* Build the apache class template and append it to the script file */
    apacheClassFile = buildApacheClass(config.pathToTemplate, config.pathToCache, config.sessionsOn, r);
    if (apacheClassFile == NULL) {
        logError("md_handler - Failed to create apache class  file", r->pool, status);
        return HTTP_INTERNAL_SERVER_ERROR;
    }
    status = apr_file_name_get(&apacheFileName, apacheClassFile);
    status = apr_file_append(apacheFileName, scriptFileName, APR_OS_DEFAULT, r->pool);
    if (status != APR_SUCCESS) {
        logError("md_handler - Failed to append apache class  file", r->pool, status);
        return HTTP_INTERNAL_SERVER_ERROR;
    }
    status = apr_file_close(apacheClassFile);
    status = apr_file_remove(apacheFileName, r->pool);

    /* Build the dart command */
    const char *exec_file = config.pathToExe;

    char *arg1 = "--package-root=";
    if (config.packageRootSet) {
        arg2 = config.packageRoot;
    } else {
        packageRoot = ap_document_root(r);
        strncat((char*) packageRoot, DEFAULT_PACKAGE, strlen(DEFAULT_PACKAGE));
        arg2 = packageRoot;
    }
    arg1 = apr_pstrcat(r->pool, arg1, arg2, NULL);

    const char *arg3 = scriptFileName;

    char* arg5 = (char *) NULL;

    const char* argv[] = {exec_file, arg1, arg3, arg5};

    /* Invoke the VM */
    fp = popen_noshell(exec_file, (const char * const *) argv, "r", &pclose_arg, 2);
    if (fp == NULL) {

        logError("md_handler - POPEN Fail ", r->pool, 0);
        return (HTTP_INTERNAL_SERVER_ERROR);
    }

    /**
     * Parse the returned output, return the actual output and
     * parse and apply the control commands. Set a default header
     * so we can see errors from the VM as we will have no control block
     */
    ap_set_content_type(r, "text/html");
    while (fgets(output, PATH_MAX, fp) != NULL)
        ap_rprintf(r, "%s", parseBuffer(output, config.sessionsOn, r));
    int pStatus = pclose_noshell(&pclose_arg);
    if (pStatus == -1) {
        logError("md_handler - POPEN Close Fail ", r->pool, 0);
        return (HTTP_INTERNAL_SERVER_ERROR);
    }

    /* Remove the script file */
    status = apr_file_close(scriptFile);
    status = apr_file_remove(scriptFileName, r->pool);

    /* Ok, all worked */
    return OK;
}

/*
 Apache register hooks 
 */
static void md_register_hooks(apr_pool_t *p) {

    config.packageRootSet = 0;
    config.sessionsOn = 0;
    ap_hook_handler(md_handler, NULL, NULL, APR_HOOK_MIDDLE);

}

/*
 Apache configuration directives 
 */
static const command_rec md_directives[] = {
    AP_INIT_TAKE1("DartExePath", md_set_exe_path, NULL, RSRC_CONF, "The path to the Dart executable"),
    AP_INIT_TAKE1("CachePath", md_set_cache_path, NULL, RSRC_CONF, "The path to the script cache"),
    AP_INIT_TAKE1("TemplatePath", md_set_template_path, NULL, RSRC_CONF, "The path to the script template"),
    AP_INIT_TAKE1("PackageRoot", md_set_package_root, NULL, RSRC_CONF, "The package root of the dart installation"),
    AP_INIT_TAKE1("Session", md_set_session, NULL, RSRC_CONF, "Sessions indicator"), {
        NULL}
};


/* 
Dispatch list for API hooks *
 */
module AP_MODULE_DECLARE_DATA dart_module = {
    STANDARD20_MODULE_STUFF,
    NULL, /* Per-directory configuration handler */
    NULL, /* Merge handler for per-directory configurations */
    NULL, /* Per-server configuration handler */
    NULL, /* Merge handler for per-server configurations */
    md_directives, /* Any directives we may have for httpd */
    md_register_hooks /* Our hook registering function */
};
