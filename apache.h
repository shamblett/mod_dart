/* 
 * File:   apache.h
 * Author: steve
 *
 * Created on 26 May 2015, 10:08
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

