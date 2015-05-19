
// Copyright 2012 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License")
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

#include <sys/stat.h>

#include "httpd.h"
#include "http_config.h"
#include "http_log.h"
#include "http_protocol.h"

extern module AP_MODULE_DECLARE_DATA dart_module;

static void dart_child_init(apr_pool_t *p, server_rec *s) {

}

static int dart_handler(request_rec *r) {

    if (strcmp(r->handler, "dart")) {
        return DECLINED;
    }

    return OK;
}

static void dart_register_hooks(apr_pool_t *p) {
    ap_hook_handler(dart_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_child_init(dart_child_init, NULL, NULL, APR_HOOK_MIDDLE);

}


static const command_rec dart_directives[] = {
    //AP_INIT_TAKE1("DartDebug", (cmd_func) dart_set_debug, NULL, OR_ALL, "Whether error messages should be sent to the browser"),
    //AP_INIT_TAKE1("DartSnapshot", (cmd_func) dart_set_snapshot, (void*) true, OR_ALL, "A dart file to be snapshotted for fast loading"),
    //AP_INIT_TAKE1("DartSnapshotForever", (cmd_func) dart_set_snapshot, (void*) false, OR_ALL, "A dart file to be snapshotted for fast loading"),
    //{ NULL },
};


/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA dart_module = {
    STANDARD20_MODULE_STUFF,
    NULL,
    NULL,
    NULL,
    NULL,
    dart_directives,
    dart_register_hooks
};