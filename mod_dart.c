
// Copyright 2012 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License")
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

#include <sys/stat.h>

#include "include/dart_api.h"

#include "bin/dartutils.h"
#include "bin/builtin.h"

#include "httpd.h"
#include "http_config.h"
#include "http_log.h"
#include "http_protocol.h"
#include "ap_config.h"
#include "apr_hash.h"
#include "apr_strings.h"

APLOG_USE_MODULE(dart);

const uint8_t snapshot_buffer[1] = {0}; // corelib, dart:io etc
char error_buffer[100];

typedef enum {
  kNull = 0,
  kNo,
  kYes
} NullableBool;

typedef struct dart_dir_config {
  NullableBool debug;
} dart_dir_config;

typedef struct dart_snapshot {
  uint8_t *buffer;
  time_t mtime;
  bool validate;
  intptr_t size;
} dart_snapshot;

typedef struct dart_server_config {
  dart_server_config *base;
  dart_snapshot master_snapshot;
  apr_hash_t *snapshots;
} dart_server_config;

extern module AP_MODULE_DECLARE_DATA dart_module;
extern "C" Dart_Handle ApacheLibraryInit(request_rec* r);
extern "C" Dart_Handle ApacheLibraryLoad();

server_rec *debug_server;

static bool IsolateCreate(const char* name, const char* main, const char* package_root, void* data, char** error) {
  
    ap_log_error(APLOG_MARK,  LOG_WARNING, 0, debug_server, "IsolateCreate - Entered");
    
    char message[128];
    sprintf(message, "IsolateCreate - Name is %s, Main is %s, Root is %s", name, main, package_root);
    ap_log_error(APLOG_MARK,  LOG_WARNING, 0, debug_server,message);
    Dart_Handle result = NULL;
  
  ap_log_error(APLOG_MARK,  LOG_WARNING, 0, debug_server, "IsolateCreate - Creating isolate");
  
  Dart_Isolate isolate = Dart_CreateIsolate(name, main, NULL, data, error);
  if (!isolate) {
      ap_log_error(APLOG_MARK,  LOG_WARNING, 0, debug_server, "IsolateCreate - Create isolate failed");
      return false;
  }
  Dart_EnterScope();
  dart::bin::Builtin::LoadAndCheckLibrary(dart::bin::Builtin::kBuiltinLibrary);
  //Builtin::SetupLibrary(Builtin::LoadLibrary(Builtin::kIOLibrary), Builtin::kIOLibrary);
  ap_log_error(APLOG_MARK,  LOG_WARNING, 0, debug_server, "IsolateCreate - Create isolate worked");
  return isolate;
}

static void IsolateShutdown(void* data) {
}

static bool IsolateInterrupt() {
  printf("Isolate interrupted!\n");
  return true;
}

Dart_Handle LoadFile(const char* cpath, struct stat *status_ptr) {
  struct stat status;
  if (stat(cpath, &status)) {
    if (errno == ENOENT) return Dart_Null();
    sprintf(error_buffer, "Couldn't stat %s: %s", cpath, strerror(errno));
    return Dart_PropagateError(Dart_NewStringFromCString(error_buffer));
  }
  if (status_ptr) *status_ptr = status;

  if (!status.st_size) return Dart_NewStringFromCString("");

  char *bytes = (char*) malloc(status.st_size + 1);
  if (!bytes) {
      sprintf(error_buffer,"Failed to malloc %ld bytes for %s: %s", status.st_size, cpath, strerror(errno));
      return Dart_PropagateError(Dart_NewStringFromCString(error_buffer));
  }
  FILE *fh = fopen(cpath, "r");
  if (!fh) {
    free(bytes);
    sprintf(error_buffer, "Failed to open %s for read: %s", cpath, strerror(errno));
    return Dart_PropagateError(Dart_NewStringFromCString(error_buffer));
  }

  int total = 0, r;
  while (total < status.st_size) {
    r = fread(&(bytes[total]), 1, status.st_size - total + 1, fh);
    if (r == 0) {
      free(bytes);
      fclose(fh);
      sprintf(error_buffer,"File was shorter: %s expected %ld got %d", cpath, status.st_size, total);
      return Dart_PropagateError(Dart_NewStringFromCString(error_buffer));
    }
    total += r;
  }

  if (!feof(fh) || total > status.st_size) {
    free(bytes);
    fclose(fh);
    sprintf(error_buffer,"File was longer: %s expected %ld", cpath, status.st_size);
    return Dart_PropagateError(Dart_NewStringFromCString(error_buffer));
  }

  fclose(fh);
  bytes[status.st_size] = 0;
  Dart_Handle result = Dart_NewStringFromCString(bytes);
  free(bytes);
  return result;
}

// Returns a malloc'd string
static char* merge_paths(const char* root, const char* relative) {
  if (*relative == '/') return strdup(relative);
  int pos = strlen(root);
  while ((pos > 0) && (root[pos-1] != '/')) pos--;
  char* result = (char*) malloc(pos + strlen(relative) + 1);
  memmove(result, root, pos);
  memmove(&(result[pos]), relative, strlen(relative) + 1); // include 0
  return result;
}

static Dart_Handle LibraryTagHandler(Dart_LibraryTag type, Dart_Handle library, Dart_Handle url) {
  if (type == Dart_kCanonicalizeUrl) return url;
  
  Dart_Handle source;
  const char* curl;
  Dart_Handle result = Dart_StringToCString(url, &curl);
  if (Dart_IsError(result)) return result;
  if (strstr(curl, ":")) {
    sprintf(error_buffer, "Unknown qualified import: %s", curl); 
    return Dart_PropagateError(Dart_NewStringFromCString(error_buffer));
  } else {
    const char* root_url;
    result = Dart_StringToCString(Dart_LibraryUrl(library), &root_url);
    if (Dart_IsError(result)) return result;
    char* path = merge_paths(root_url, curl);
    source = LoadFile(path, NULL);
    if (Dart_IsNull(source)) {
        sprintf(error_buffer,"File %s was not found\n", curl);
       return Dart_PropagateError(Dart_NewStringFromCString(error_buffer));
    }
    free(path);
    if (Dart_IsError(source)) return source;
  }

  if (Dart_IsError(source)) return source;
  if (type == Dart_kSourceTag) return Dart_LoadSource(library, url, source, 0, 0);
  return Dart_LoadLibrary(url, source, 0, 0);
}

static Dart_Handle MasterSnapshotLibraryTagHandler(Dart_LibraryTag type, Dart_Handle library, Dart_Handle url) {
  if (type == Dart_kCanonicalizeUrl) return url;
  if (type == Dart_kSourceTag)  {
      sprintf(error_buffer,"Unexpected #source in master snapshot libraries");
      return Dart_PropagateError(Dart_NewStringFromCString(error_buffer));
  }
  const char* curl;
  Dart_Handle result = Dart_StringToCString(url, &curl);
  if (Dart_IsError(result)) return result;
  return result;
  /*if (!strcmp(curl, "dart:utf")) {
    return Builtin::LoadLibrary(Builtin::kUtfLibrary);
  } else if (!strcmp(curl, "dart:uri")) {
    return Builtin::LoadLibrary(Builtin::kUriLibrary);
  } else if (!strcmp(curl, "dart:io")) {
    return Builtin::LoadLibrary(Builtin::kIOLibrary);
  } else if (!strcmp(curl, "dart:crypto")) {
    return Builtin::LoadLibrary(Builtin::kCryptoLibrary);
  } else if (!strcmp(curl, "dart:json")) {
    return Builtin::LoadLibrary(Builtin::kJsonLibrary);
  } else {
    return Dart_PropagateError("Unknown qualified import %s", curl);
  }*/
}

static Dart_Handle ScriptSnapshotLibraryTagHandler(Dart_LibraryTag type, Dart_Handle library, Dart_Handle url) {
  if (type == Dart_kImportTag || type == Dart_kSourceTag) {
    const char* curl;
    Dart_Handle result = Dart_StringToCString(url, &curl);
    if (Dart_IsError(result)) return result;
    if (type == Dart_kImportTag && !strcmp(curl, "apache:handler")) {
      return ApacheLibraryLoad();
    } else if (!strstr(curl, ":")) {
      return LibraryTagHandler(type, library, url);
    }
  }
  return MasterSnapshotLibraryTagHandler(type, library, url);
}

static bool dart_isolate_create(request_rec *r) {
  char* error;
  if (!IsolateCreate("name", "main", NULL,(void*) r, &error)) {
    ap_log_rerror(APLOG_MARK, LOG_WARNING, 0, r, "Failed to create isolate: %s", error);
    return false;
  }
  Dart_EnterScope();
  Dart_SetLibraryTagHandler(LibraryTagHandler);
  Dart_Handle result = ApacheLibraryInit(r);
  if (Dart_IsError(result)) {
    ap_log_rerror(APLOG_MARK, LOG_WARNING, 0, r, "Failed to initialize Apache library: %s", Dart_GetError(result));
    return false;
  }
  return true;
}

static apr_status_t dart_isolate_destroy(void* ctx) {
  Dart_ExitScope();
  Dart_ShutdownIsolate();
  return OK;
}

static bool isCurrent(char* filename, dart_snapshot *snapshot) {
  if (!snapshot->validate) return true;
  struct stat status;
  if (stat(filename, &status)) return false;
  return snapshot->mtime >= status.st_mtime;
}

static bool isDebug(request_rec *r) {
  dart_dir_config *cfg = (dart_dir_config*) ap_get_module_config(r->per_dir_config, &dart_module);
  return cfg->debug == kYes;
}

static dart_snapshot *getScriptSnapshot(request_rec *r) {
  dart_server_config *cfg = (dart_server_config*) ap_get_module_config(r->server->module_config, &dart_module);
  while (cfg->base) cfg = cfg->base;
  dart_snapshot* result = (dart_snapshot*) apr_hash_get(cfg->snapshots, r->filename, APR_HASH_KEY_STRING);
  if (!result) {
    if (isDebug(r)) apr_table_set(r->headers_out, "X-Dart-Snapshot", "No; None configured");
    return NULL;
  }
  if (!result->buffer) {
    if (isDebug(r)) apr_table_set(r->headers_out, "X-Dart-Snapshot", "No; Failed to create snapshot at startup");
    return NULL;
  }
  if (!isCurrent(r->filename, result)) {
    if (isDebug(r)) apr_table_set(r->headers_out, "X-Dart-Snapshot", "No; Snapshot is stale");
    return NULL;
  }
  if (isDebug(r)) apr_table_set(r->headers_out, "X-Dart-Snapshot", "Yes");
  return result;
}

static bool initializeState = false;
static void dart_child_init(apr_pool_t *p, server_rec *s) {
    return;
  ap_log_error(APLOG_MARK, LOG_WARNING, 0, s, "Dart_Child_Init - Entered");
  bool initializeState1 = false;
  bool initializeState2 =false;
  ap_log_error(APLOG_MARK, LOG_WARNING, 0, s, "Dart_Child_Init -Init 1");
  initializeState1 = Dart_SetVMFlags(0, NULL);
  ap_log_error(APLOG_MARK, LOG_WARNING, 0, s, "Dart_Child_Init -Init 2");
  initializeState2 = Dart_Initialize(NULL, (Dart_IsolateCreateCallback)IsolateCreate, 
          (Dart_IsolateInterruptCallback)IsolateInterrupt, NULL, (Dart_IsolateShutdownCallback)IsolateShutdown,
          NULL, NULL, NULL, NULL, NULL);
  
  // This is allowed to fail, we may have already initialized the VM for snapshot creation
  initializeState |= (Dart_SetVMFlags(0, NULL) && Dart_Initialize(NULL, (Dart_IsolateCreateCallback)IsolateCreate, 
          (Dart_IsolateInterruptCallback)IsolateInterrupt, NULL, (Dart_IsolateShutdownCallback)IsolateShutdown,
          NULL, NULL, NULL, NULL, NULL));
  initializeState = initializeState1 | initializeState2;
  ap_log_error(APLOG_MARK, LOG_WARNING, 0, s, "Dart_Child_Init - Exited");
} 

static int fatal(request_rec *r, const char *format, Dart_Handle error) {
  ap_log_rerror(APLOG_MARK, LOG_WARNING, 0, r, format, Dart_GetError(error));
  if (!isDebug(r)) return HTTP_INTERNAL_SERVER_ERROR;
  r->content_type = "text/plain";
  r->status = HTTP_INTERNAL_SERVER_ERROR;
  ap_rprintf(r, format, Dart_GetError(error));
  ap_rprintf(r, "\n");
  return OK;  
}

static int dart_handler(request_rec *r) {
  if (strcmp(r->handler, "dart")) {
    return DECLINED;
  }
  if (!initializeState) {
    ap_log_rerror(APLOG_MARK, LOG_WARNING, 0, r, "Failed to initialize dart VM at startup");
    return HTTP_INTERNAL_SERVER_ERROR;
  }
  if (!dart_isolate_create(r)) return HTTP_INTERNAL_SERVER_ERROR;
  apr_pool_cleanup_register(r->pool, NULL, dart_isolate_destroy, apr_pool_cleanup_null);
  dart_snapshot *snapshot = getScriptSnapshot(r);
  Dart_Handle library;
  if (snapshot) {
    library = Dart_LoadScriptFromSnapshot(snapshot->buffer, snapshot->size );
  } else {
    Dart_Handle script = LoadFile(r->filename, NULL);
    if (Dart_IsNull(script)) return HTTP_NOT_FOUND;
    library = Dart_IsError(script) ? script : Dart_LoadScript(Dart_NewStringFromCString(r->filename), script, 0, 0);
  }
  if (Dart_IsError(library)) return fatal(r, "Failed to load script: %s", library);
  Dart_Handle result = Dart_Invoke(library, Dart_NewStringFromCString("main"), 0, NULL);
  if (Dart_IsError(result)) return fatal(r, "Failed to execute main(): %s", result);
  return OK;
}

Dart_Handle create_master_snapshot(apr_pool_t *pool, dart_snapshot *target, const char* name) {
  Dart_SetLibraryTagHandler(MasterSnapshotLibraryTagHandler);
  Dart_Handle result = NULL;
  //Dart_Handle result = Builtin::LoadLibrary(Builtin::kBuiltinLibrary);
  if (Dart_IsError(result)) return result;
  result = ApacheLibraryLoad();
  if (Dart_IsError(result)) return result;
  uint8_t *buffer;
  intptr_t size;
  result = Dart_CreateSnapshot(NULL, NULL, &buffer, &size);
  if (Dart_IsError(result)) return result;
  target->buffer = (uint8_t*) apr_pcalloc(pool, size); // This lives forever
  if (!target->buffer) {    
      sprintf(error_buffer,"Failed to allocate %ld bytes for master snapshot", size);
      return Dart_PropagateError(Dart_NewStringFromCString(error_buffer));  
  }
  memmove(target->buffer, buffer, size);
  target->mtime = 0;
  target->size = size;
  fprintf(stderr, "mod_dart: Created master snapshot: %ld bytes\n", size);
  return Dart_Null();
}

Dart_Handle create_script_snapshot(apr_pool_t *pool, dart_snapshot *target, const char *name) {
  Dart_SetLibraryTagHandler(ScriptSnapshotLibraryTagHandler);
  struct stat status;
  Dart_Handle result = LoadFile(name, &status);
  if (Dart_IsNull(result)) {
      sprintf(error_buffer,"Script not found: %s", name);
      return Dart_PropagateError(Dart_NewStringFromCString(error_buffer));
  }
  if (Dart_IsError(result)) return result;
  result = Dart_LoadScript(Dart_NewStringFromCString(name), result, 0, 0);
  if (Dart_IsError(result)) return result;
  uint8_t *buffer;
  intptr_t size;
  result = Dart_CreateScriptSnapshot(&buffer, &size);
  if (Dart_IsError(result)) return result;
  target->buffer = (uint8_t*) apr_pcalloc(pool, size); // This lives forever
  if (!target->buffer) {
      sprintf(error_buffer, "Failed to allocate %ld bytes for snapshot of %s", size, name);
      return Dart_PropagateError(Dart_NewStringFromCString(error_buffer));
  }
      memmove(target->buffer, buffer, size);
  target->mtime = status.st_mtime;
  target->size = size;
  fprintf(stderr, "mod_dart: Created snapshot of %s: %ld bytes\n", name, size);
  return Dart_Null();
}

bool create_snapshot(apr_pool_t *pool, dart_snapshot* target, const char* name, uint8_t *base_snapshot, Dart_Handle (*creator)(apr_pool_t *pool, dart_snapshot *target, const char* name), char **error) {
  *error = NULL;
  
  ap_log_error(APLOG_MARK,  LOG_WARNING, 0, debug_server, "create_snapshot - Creating isolate"); 
  Dart_Isolate isolate = NULL;
  if (base_snapshot) {
    isolate = Dart_CreateIsolate(name, "main", base_snapshot, NULL, error);
  } else {
     isolate = Dart_CreateIsolate(name, "main", snapshot_buffer, NULL, error);
  } 
  if (!isolate) {
      ap_log_error(APLOG_MARK,  LOG_WARNING, 0, debug_server, "create_snapshot - Creating isolate failed ");
      return false;
  }
  ap_log_error(APLOG_MARK,  LOG_WARNING, 0, debug_server, "create_snapshot = Isolate Created"); 
  Dart_EnterScope();
   ap_log_error(APLOG_MARK,  LOG_WARNING, 0, debug_server, "create_snapshot = Calling creator"); 
  Dart_Handle result = creator(pool, target, name);
  if (Dart_IsError(result)) *error = apr_pstrdup(pool, Dart_GetError(result));
 // Dart_ExitScope();
  //Dart_ShutdownIsolate();
  ap_log_error(APLOG_MARK,  LOG_WARNING, 0, debug_server, "create_snapshot - Exiting");
  return *error == NULL;
}

int dart_snapshots(apr_pool_t *pconf, apr_pool_t *plog, apr_pool_t *ptemp, server_rec *server) {
   
  // Modules are loaded twice, only actually create the snapshot on second load
  ap_log_error(APLOG_MARK,  LOG_WARNING, 0, server, "Dart_Snapshots - Entered");  
  debug_server = server;
  void *data = NULL;
  apr_pool_userdata_get(&data, "dart_dryrun_complete", server->process->pool);
  if (!data) { // This is the dry-run
     ap_log_error(APLOG_MARK,  LOG_WARNING, 0, server, "Dart_Snapshots - Dry run");   
    apr_pool_userdata_set((const void*) 1, "dart_dryrun_complete", apr_pool_cleanup_null, server->process->pool);
    return OK;
  }
  
  ap_log_error(APLOG_MARK, LOG_WARNING, 0, server, "Dart_Snapshots - Processing");  
  // Only create snapshots in the root server
  dart_server_config *cfg = (dart_server_config*) ap_get_module_config(server->module_config, &dart_module);
  if (cfg->base) return OK;
  ap_log_error(APLOG_MARK, LOG_WARNING, 0, server, "Dart_Snapshots -Initializing");  
  initializeState = Dart_SetVMFlags(0, NULL) && Dart_Initialize(NULL, (Dart_IsolateCreateCallback)IsolateCreate, 
          (Dart_IsolateInterruptCallback)IsolateInterrupt, NULL, (Dart_IsolateShutdownCallback)IsolateShutdown,
          NULL,//dart::bin::DartUtils::OpenFile, 
          NULL,//dart::bin::DartUtils::ReadFile, 
          NULL,//dart::bin::DartUtils::WriteFile, 
          NULL,//dart::bin::DartUtils::CloseFile, 
          NULL);//dart::bin::DartUtils::EntropySource);

  char* error;
 /* ap_log_error(APLOG_MARK, LOG_WARNING, 0, server, "Dart_Snapshots - Creating Master Snapshot");  
  if (!create_snapshot(server->process->pool, &(cfg->master_snapshot), "master", NULL, create_master_snapshot, &error)) {
    ap_log_error(APLOG_MARK, LOG_ERR, 0, server, "mod_dart: Master snapshot failed: %s", error);
   return 1;
  }
  
  ap_log_error(APLOG_MARK, LOG_WARNING, 0, server, "Dart_Snapshots - Creating Script Snapshot");  
  dart_snapshot *val;
  const void *key;
  for (apr_hash_index_t *p = apr_hash_first(ptemp, cfg->snapshots); p; p = apr_hash_next(p)) {
    apr_hash_this(p, &key, NULL, (void**) &val);
    // TODO use pconf instead of server->process->pool?
    if (!create_snapshot(server->process->pool, val, (const char*) key, cfg->master_snapshot.buffer, create_script_snapshot, &error)) {
      ap_log_error(APLOG_MARK, LOG_WARNING, 0, server, "mod_dart: Script snapshot failed for %s: %s", (const char*) key, error);
      val->buffer = NULL;
      val->mtime = 0;
    }
  }
*/
  ap_log_error(APLOG_MARK,  LOG_WARNING, 0, server, "Dart_Snapshots - Exiting");  
  return OK;
}

static void dart_register_hooks(apr_pool_t *p) {
  ap_hook_handler(dart_handler, NULL, NULL, APR_HOOK_MIDDLE);
  ap_hook_child_init(dart_child_init, NULL, NULL, APR_HOOK_MIDDLE);
  ap_hook_post_config(dart_snapshots, NULL, NULL, APR_HOOK_MIDDLE);
  
}

static const char *dart_set_debug(cmd_parms *cmd, void *cfg_, const char *arg) {
  dart_dir_config *cfg = (dart_dir_config*) cfg_;
  cfg->debug = strcasecmp("on", arg) ? kNo : kYes;
  return NULL;
}

static const char *dart_set_snapshot(cmd_parms *cmd, void *cfg_, const char *arg, const char *arg2) {
  dart_server_config *cfg = (dart_server_config*) ap_get_module_config(cmd->server->module_config, &dart_module);
  while (cfg->base) cfg = cfg->base;
  dart_snapshot *snapshot = (dart_snapshot *) apr_pcalloc(apr_hash_pool_get(cfg->snapshots), sizeof(dart_snapshot));
  snapshot->buffer = NULL;
  snapshot->mtime = 0;
  snapshot->validate = (bool) cmd->info;
  apr_hash_set(cfg->snapshots, arg, APR_HASH_KEY_STRING, (void*) snapshot);
  return NULL;
}

static const command_rec dart_directives[] = {
  AP_INIT_TAKE1("DartDebug", (cmd_func) dart_set_debug, NULL, OR_ALL, "Whether error messages should be sent to the browser"),
  AP_INIT_TAKE1("DartSnapshot", (cmd_func) dart_set_snapshot, (void*) true, OR_ALL, "A dart file to be snapshotted for fast loading"),
  AP_INIT_TAKE1("DartSnapshotForever", (cmd_func) dart_set_snapshot, (void*) false, OR_ALL, "A dart file to be snapshotted for fast loading"),
  { NULL },
};

static void *create_dir_conf(apr_pool_t* pool, char* context_) {
  dart_dir_config *cfg = (dart_dir_config*) apr_pcalloc(pool, sizeof(dart_dir_config));
  if (cfg) {
    cfg->debug = kNull;
  }
  return cfg;
}

static void *merge_dir_conf(apr_pool_t* pool, void* base_, void* add_) {
  dart_dir_config *base = (dart_dir_config*) base_;
  dart_dir_config *add = (dart_dir_config*) add_;
  dart_dir_config *cfg = (dart_dir_config*) apr_pcalloc(pool, sizeof(dart_dir_config));
  cfg->debug = add->debug ? add->debug : base->debug;
  return cfg;
}

static void *create_server_conf(apr_pool_t* pool, server_rec *server) {
  dart_server_config *cfg = (dart_server_config*) apr_pcalloc(pool, sizeof(dart_server_config));
  if (cfg) {
    cfg->base = NULL;
    cfg->snapshots = apr_hash_make(pool);
  }
  return cfg;
}

static void *merge_server_conf(apr_pool_t *pool, void *base_, void *add_) {
  dart_server_config *base = (dart_server_config*) base_;
  dart_server_config *add = (dart_server_config*) add_;
  dart_server_config *cfg = (dart_server_config*) apr_pcalloc(pool, sizeof(dart_server_config));
  cfg->base = base;
  cfg->snapshots = NULL;
  while (base->base) base = base->base;
  void *val;
  const void *key;
  for (apr_hash_index_t *p = apr_hash_first(pool, add->snapshots); p; p = apr_hash_next(p)) {
    apr_hash_this(p, &key, NULL, &val);
    apr_hash_set(base->snapshots, key, APR_HASH_KEY_STRING, val);
  }
  return cfg;
}

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA dart_module = {
  STANDARD20_MODULE_STUFF, 
  create_dir_conf,
  merge_dir_conf,
  create_server_conf,
  merge_server_conf,
  dart_directives,
  dart_register_hooks
};