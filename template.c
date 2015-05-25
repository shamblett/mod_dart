#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#include <apr_file_io.h>

#include "template.h"
#include "ctemplate-1.0/ctemplate.h"

char* apacheClass = "//--------- Apache Class Start -----\nclass Apache{ static final String ip='333.444.555.666';}";
const char* templatePath = NULL;
const char* cachePath = NULL;

TMPL_varlist * varList;

char* getApacheClass() {
   
    char scriptPath[PATH_MAX];
    FILE *fp;
    int fd;
    char* buffer = malloc(1000);
    int status = -1;
    apr_file_t* thefile;
    apr_size_t fileSize = 1000;

    strcpy(scriptPath, cachePath);
    strcat(scriptPath, "XXXXXX");
    fd = mkstemp(scriptPath);
    if ( !fd ) { return "MKSTMP failure";}
    fp = fdopen(fd, "rw");
    fclose(fp);
    fp = fopen(scriptPath, "w");
    status = TMPL_write(templatePath, 0, 0, varList, fp, 0);
    fclose(fp);
    if ( status ) { return "TMPL_WRITE failed";}
    thefile = apr_file_open(scriptPath, "r");
    apr_file_read(thefile, buffer, &fileSize);
                                       

    TMPL_free_varlist (varList);
    return buffer;

}

void setTemplatePath(const char* path) {

    templatePath = path;
}

void setCachePath(const char* path) {

    cachePath = path;
}

void addVar(const char *name, const char *value) {

    varList = TMPL_add_var(varList, name, value, 0);
}