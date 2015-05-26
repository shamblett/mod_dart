
#include "error.h"

#include "utils.h"

apr_file_t* getTempFile(char* template, apr_pool_t* pool) {
    
    apr_status_t status;
    apr_file_t *ret = NULL;

    
    status = apr_file_mktemp(&ret, template, 0, pool);
    if ( status != APR_SUCCESS) {
        logError("mod_dart - getTempFile - Failed to create temp file", pool, status);
        return NULL;
    }
    
    return ret;

}
