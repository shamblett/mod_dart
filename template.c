#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

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
    ssize_t bytes_read = 0;
    int status = -1;

    strcpy(scriptPath, cachePath);
    strcat(scriptPath, "XXXXXX");
    fd = mkstemp(scriptPath);
    if ( !fd ) { return "MKSTMP failure";}
    fp = fdopen(fd, "w+");
    
    status = TMPL_write(templatePath, 0, 0, varList, fp, 0);
    if ( status ) { return "TMPL_WRITE failed";}
    bytes_read = getdelim(&buffer, 0, '\0', fp);
    if (bytes_read != -1) {return "getdelim failed";}

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