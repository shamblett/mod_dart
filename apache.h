/*
 * Package : mod_dart
 * Author : S. Hamblett <steve.hamblett@linux.com>
 * Date   : 26/05/2015
 * Copyright :  S.Hamblett 2015
 * License : GPL V3, see the LICENSE file for details
 */


#ifndef APACHE_H
#define	APACHE_H

#include <httpd.h>
#include <http_config.h>
#include <http_log.h>
#include <http_protocol.h>
#include <apr_file_io.h> 


#ifdef	__cplusplus
extern "C" {
#endif

apr_file_t* buildApacheClass(char* templatePath, char* cachePath, request_rec *r);


#ifdef	__cplusplus
}
#endif

#endif	/* APACHE_H */

