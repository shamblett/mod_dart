/*
 * Package : mod_dart
 * Author : S. Hamblett <steve.hamblett@linux.com>
 * Date   : 26/05/2015
 * Copyright :  S.Hamblett 2015
 * License : GPL V3, see the LICENSE file for details
 */

#include <ctype.h>

#include "error.h"

#include "utils.h"

apr_file_t* getTempFile(char* template, apr_pool_t* pool) {

    apr_status_t status;
    apr_file_t *ret = NULL;


    status = apr_file_mktemp(&ret, template, 0, pool);
    if (status != APR_SUCCESS) {
        logError("getTempFile - Failed to create temp file", pool, status);
        return NULL;
    }

    return ret;

}

char* trimWhiteSpace(char *s) {

    int i = 0;
    int j = strlen(s) - 1;
    int k = 0;

    while (isspace(s[i]) && s[i] != '\0')
        i++;

    while (isspace(s[j]) && j >= 0)
        j--;

    while (i <= j)
        s[k++] = s[i++];

    s[k] = '\0';

    return s;
}
