/*
 * Package : mod_dart
 * Author : S. Hamblett <steve.hamblett@linux.com>
 * Date   : 26/05/2015
 * Copyright :  S.Hamblett 2015
 * License : GPL V3, see the LICENSE file for details
 */


#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#include <apr_file_io.h>


#include "utils.h"
#include "ctemplate-1.0/ctemplate.h"

#include "template.h"

TMPL_varlist * addVar(const char *name, const char *value, TMPL_varlist * varList ) {

    return TMPL_add_var(varList, name, value, 0);
}