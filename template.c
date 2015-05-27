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

tpl_varlist * tpl_addVar(const char *name, const char *value, tpl_varlist * varList) {

    return TMPL_add_var(varList, name, value, 0);
}

int tpl_write(const char *filename, const tpl_varlist *varlist, FILE *out) {

    return TMPL_write(filename, 0, 0, varlist, out, 0);
}

void tpl_free(tpl_varlist *varlist) {

    TMPL_free_varlist(varlist);
}