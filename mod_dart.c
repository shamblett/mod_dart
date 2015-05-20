
// Copyright 2012 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License")
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

#include <stdio.h>

#include "httpd.h"
#include "http_config.h"
#include "http_log.h"
#include "http_protocol.h"

#include "mod_dart.h"

//static void md_child_init(apr_pool_t *p, server_rec *s) {

//}

static int md_handler(request_rec *r) {
 
    if (!r->handler || strcmp(r->handler,"dart")) return (DECLINED);
    
    /* Now that we are handling this request, we'll write out "Hello, world!" to the client.
     * To do so, we must first set the appropriate content type, followed by our output.
     */
    ap_set_content_type(r, "text/html");
    ap_rprintf(r, "Hello, world!");
    
    /* Lastly, we must tell the server that we took care of this request and everything went fine.
     * We do so by simply returning the value OK to the server.
     */
    return OK;
}

static void md_register_hooks(apr_pool_t *p) {
    ap_hook_handler(md_handler, NULL, NULL, APR_HOOK_MIDDLE);
    //ap_hook_child_init(md_child_init, NULL, NULL, APR_HOOK_MIDDLE);

}


static const command_rec md_directives[] = {
    //AP_INIT_TAKE1("DartDebug", (cmd_func) dart_set_debug, NULL, OR_ALL, "Whether error messages should be sent to the browser"),
    
    { NULL }
};


/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA dart_module = {
STANDARD20_MODULE_STUFF,
    NULL, /* Per-directory configuration handler */
    NULL,  /* Merge handler for per-directory configurations */
    NULL, /* Per-server configuration handler */
    NULL,  /* Merge handler for per-server configurations */
    md_directives,      /* Any directives we may have for httpd */
    md_register_hooks   /* Our hook registering function */
};
