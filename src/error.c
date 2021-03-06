/*
 * Package : mod_dart
 * Author : S. Hamblett <steve.hamblett@linux.com>
 * Date   : 26/05/2015
 * Copyright :  S.Hamblett 2015
 * License : GPL V3, see the LICENSE file for details
 */

#include "error.h" 

void logError(char* message, apr_pool_t* pool, apr_status_t status) {

    ap_log_perror(APLOG_MARK, APLOG_ERR, status, pool,
            message);

}
