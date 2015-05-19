
// Copyright 2012 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License")
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

#include <sys/stat.h>

#include "httpd.h"
#include "http_config.h"
#include "http_log.h"
#include "http_protocol.h"

#include "mod_dart.h"

static void child_init(apr_pool_t *p, server_rec *s) {

}

static int handler(request_rec *r) {

    if (strcmp(r->handler, "dart")) {
        return DECLINED;
    }

    return OK;
}

static void register_hooks(apr_pool_t *p) {
    ap_hook_handler(handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_child_init(child_init, NULL, NULL, APR_HOOK_MIDDLE);

}


static const command_rec directives[] = {
    //AP_INIT_TAKE1("DartDebug", (cmd_func) dart_set_debug, NULL, OR_ALL, "Whether error messages should be sent to the browser"),
    //AP_INIT_TAKE1("DartSnapshot", (cmd_func) dart_set_snapshot, (void*) true, OR_ALL, "A dart file to be snapshotted for fast loading"),
    //AP_INIT_TAKE1("DartSnapshotForever", (cmd_func) dart_set_snapshot, (void*) false, OR_ALL, "A dart file to be snapshotted for fast loading"),
    //{ NULL },
};

static void *create_dir_conf(apr_pool_t* pool, char* context_) {
  
}

static void *merge_dir_conf(apr_pool_t* pool, void* base_, void* add_) {
  
}

static void *create_server_conf(apr_pool_t* pool, server_rec *server) {
  
}

static void *merge_server_conf(apr_pool_t *pool, void *base_, void *add_) {

}

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA dart_module = {
STANDARD20_MODULE_STUFF,
    create_dir_conf, /* Per-directory configuration handler */
    merge_dir_conf,  /* Merge handler for per-directory configurations */
    create_server_conf, /* Per-server configuration handler */
    merge_server_conf,  /* Merge handler for per-server configurations */
    directives,      /* Any directives we may have for httpd */
    register_hooks   /* Our hook registering function */
};
