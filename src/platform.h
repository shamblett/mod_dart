/*
 * Package : mod_dart
 * Author : S. Hamblett <steve.hamblett@linux.com>
 * Date   : 26/05/2015
 * Copyright :  S.Hamblett 2015
 * License : GPL V3, see the LICENSE file for details
 */

/* Purpose :-
 * 
 * This module contains the platform specific setup 
 */

#ifndef PLATFORM_H
#define	PLATFORM_H


#ifdef	__cplusplus
extern "C" {
#endif

#include <httpd.h>
#include <http_config.h>
#include <http_log.h>
#include <http_protocol.h>
#include <http_core.h>
#include <ap_config.h>
#include <ap_provider.h>
#include <apr_strings.h>
#include <apr_hash.h>
#include <apr_file_io.h>
#include <apr_strings.h>
#include <apr_pools.h>


#ifdef	__cplusplus
}
#endif

#endif	/* PLATFORM_H */

