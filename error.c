
#include <httpd.h>
#include <http_config.h>
#include <http_log.h>

#include "error.h" 

void logError(char* message, apr_pool_t* pool, apr_status_t status) {
    
    ap_log_perror(APLOG_MARK, APLOG_ERR, status, pool,
        message);
    
}
