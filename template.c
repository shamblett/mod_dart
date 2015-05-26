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