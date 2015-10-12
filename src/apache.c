/*
 * Package : mod_dart
 * Author : S. Hamblett <steve.hamblett@linux.com>
 * Date   : 26/05/2015
 * Copyright :  S.Hamblett 2015
 * License : GPL V3, see the LICENSE file for details
 */

#include "template.h"
#include "utils.h"
#include "error.h"
#include "constants.h"
#include "version.h"
#include "../jansson/jansson.h"
#include "session.h"
#include "../apreq/apache_request.h"

#include "apache.h"

/* Parse form data from a string. The input string is NOT preserved. */
static apr_hash_t *parse_form_from_string(request_rec *r, char *args) {

    apr_hash_t *form = NULL;
    char *pair;
    char *eq;
    const char *delim = "&";
    char *last;
    if (args == NULL) {
        return NULL;
    }

    if (args != NULL) {

        form = apr_hash_make(r->pool);

        /* Split the input on '&' */
        for (pair = apr_strtok(args, delim, &last); pair != NULL;
                pair = apr_strtok(NULL, delim, &last)) {
            for (eq = pair; *eq; ++eq) {
                if (*eq == '+') {
                    *eq = ' ';
                }
            }
            /* split into Key / Value and unescape it */
            eq = strchr(pair, '=');
            if (eq) {
                *eq++ = '\0';
                ap_unescape_url(pair);
                ap_unescape_url(eq);
            } else {
                eq = "";
                ap_unescape_url(pair);
            }
            /* Store key/value pair in our form hash. */
            apr_hash_set(form, pair, APR_HASH_KEY_STRING, eq);

        }
    }

    return form;
}

/* Parse cookie data from a string. The input string is NOT preserved. */
static apr_hash_t *parse_cookie_from_string(request_rec *r, const char *args) {

    apr_hash_t *cookies = NULL;
    char *pair;
    char *eq;
    const char *delim = ";";
    char *last;
    if (args == NULL) {
        return NULL;
    }

    if (args != NULL) {

        cookies = apr_hash_make(r->pool);

        /* Split the input on ';' */
        for (pair = apr_strtok((char*) args, delim, &last); pair != NULL;
                pair = apr_strtok(NULL, delim, &last)) {
            for (eq = pair; *eq; ++eq) {
                if (*eq == '+') {
                    *eq = ' ';
                }
            }
            /* split into Key / Value and unescape it */
            eq = strchr(pair, '=');
            if (eq) {
                *eq++ = '\0';
                ap_unescape_url(pair);
                ap_unescape_url(eq);
            } else {
                eq = "";
                ap_unescape_url(pair);
            }
            /* Store key/value pair in our form hash. */
            pair = trimWhiteSpace(pair);
            apr_hash_set(cookies, pair, APR_HASH_KEY_STRING, eq);

        }
    }

    return cookies;
}

tpl_varlist* getCookiesGlobal(request_rec* r, tpl_varlist* varlist) {

    apr_hash_index_t *hi;
    void *val;
    const void *key;
    tpl_loop *loop = NULL;
    const char *cookies;

    cookies = apr_table_get(r->headers_in, "Cookie");
    if (cookies) {

        apr_hash_t* getHash = parse_cookie_from_string(r, cookies);
        if (getHash != NULL) {
            for (hi = apr_hash_first(r->pool, getHash); hi; hi = apr_hash_next(hi)) {
                apr_hash_this(hi, &key, NULL, &val);
                loop = tpl_addVarList(loop, tpl_addLoopVar(
                        "key", key, "val", val));
            }


            return tpl_addLoop(varlist, "cookie_map", loop);
        }
    }

    return varlist;
}

tpl_varlist* getGetGlobal(request_rec* r, tpl_varlist* varlist) {

    apr_hash_index_t *hi;
    void *val;
    const void *key;
    tpl_loop *loop = NULL;

    apr_hash_t* getHash = parse_form_from_string(r, r->args);
    if (getHash != NULL) {
        for (hi = apr_hash_first(r->pool, getHash); hi; hi = apr_hash_next(hi)) {
            apr_hash_this(hi, &key, NULL, &val);
            loop = tpl_addVarList(loop, tpl_addLoopVar(
                    "key", key, "val", val));
        }


        return tpl_addLoop(varlist, "get_map", loop);
    }

    return varlist;
}

tpl_varlist* getPostGlobal(request_rec* r, tpl_varlist* varlist) {

    tpl_loop *loop = NULL;
    apr_table_t* postParams;

    int postCallback(void* r, const char* key, const char* value) {

        loop = tpl_addVarList(loop, tpl_addLoopVar(
                "key", key, "val", value));
        return 1;
    }

    ApacheRequest* postReq = ApacheRequest_new(r, NULL);
    ApacheRequest_parse_urlencoded(postReq);
    postParams = ApacheRequest_post_params(postReq, r->pool);

    apr_table_do(postCallback, r, postParams, NULL);


    return tpl_addLoop(varlist, "post_map", loop);
}

tpl_varlist* getPostGlobalMultiPart(request_rec* r, tpl_varlist* varlist, const char* cachePath) {

    tpl_loop *loop = NULL;
    apr_table_t* postParams;
    tpl_varlist* retVarList = NULL;

    int postCallback(void* r, const char* key, const char* value) {

        loop = tpl_addVarList(loop, tpl_addLoopVar(
                "key", key, "val", value));
        return 1;
    }

    /* Parse the multi part form */
    ApacheRequest* postReq = ApacheRequest_new(r, cachePath);
    ApacheRequest_parse_multipart(postReq);

    /* Check for uploaded files */
    ApacheUpload *upload = ApacheRequest_upload(postReq);
    if (upload != NULL) {

        /* Construct the FILES superglobal */
        char* val;
        tpl_loop *fileLoop = NULL;
        while (upload != NULL) {

            const char* fieldName = upload->name;
            const char* name = upload->filename;
            const char* type = ApacheUpload_type(upload);
            const char* tmp_name = upload->tempname;
            const char* size = apr_ltoa(r->pool, upload->size);
            val = " { ";
            val = apr_pstrcat(r->pool, val, "'name' : '", NULL);
            val = apr_pstrcat(r->pool, val, name, NULL);
            val = apr_pstrcat(r->pool, val, "' , 'type' : '", NULL);
            val = apr_pstrcat(r->pool, val, type, NULL);
            val = apr_pstrcat(r->pool, val, "' , 'size' : '", NULL);
            val = apr_pstrcat(r->pool, val, size, NULL);
            val = apr_pstrcat(r->pool, val, "' ,'tmp_name' : '", NULL);
            val = apr_pstrcat(r->pool, val, tmp_name, NULL);
            val = apr_pstrcat(r->pool, val, "' } ", NULL);
            fileLoop = tpl_addVarList(fileLoop, tpl_addLoopVar(
                    "key", fieldName, "val", val));

            upload = upload->next;
        }
        retVarList = tpl_addLoop(varlist, "file_map", fileLoop);

    }

    /* Get the post params and return them */
    postParams = ApacheRequest_post_params(postReq, r->pool);
    apr_table_do(postCallback, r, postParams, NULL);
    if (retVarList == NULL) {
        retVarList = tpl_addLoop(varlist, "post_map", loop);
    } else {
        retVarList = tpl_addLoop(retVarList, "post_map", loop);
    }
    return retVarList;
}

tpl_varlist* getRequestHeaders(request_rec* r, tpl_varlist* varlist) {


    tpl_loop *loop = NULL;

    int headerCallback(void* r, const char* key, const char* value) {

        loop = tpl_addVarList(loop, tpl_addLoopVar(
                "key", key, "val", value));
        return 1;
    }

    apr_table_do(headerCallback, r, r->headers_in, NULL);

    return tpl_addLoop(varlist, "request_header_map", loop);
}

tpl_varlist* getServerGlobal(request_rec* r, tpl_varlist* varlist) {

    apr_status_t status;
    char ctime[APR_CTIME_LEN];
    char* addr;

    /* SELF */
    const char* self = apr_pstrdup(r->pool, r->filename);
    varlist = tpl_addVar("server_self", basename(self), varlist);

    /* SERVER_ADDR */
    status = apr_sockaddr_ip_get(&addr, r->connection->local_addr);
    if (status != APR_SUCCESS) {

        varlist = tpl_addVar("server_addr", "Unable To Obtain", varlist);

    } else {

        varlist = tpl_addVar("server_addr", addr, varlist);

    }

    /* SERVER_NAME */
    const char* hostname = apr_pstrdup(r->pool, r->server->server_hostname);
    varlist = tpl_addVar("server_name", hostname, varlist);

    /* SERVER_SOFTWARE */
    const char* serverSoftware = ap_get_server_description();
    varlist = tpl_addVar("server_software", serverSoftware, varlist);

    /* SERVER_PROTOCOL */
    const char* protocol = apr_pstrdup(r->pool, r->protocol);
    varlist = tpl_addVar("server_protocol", protocol, varlist);

    /* REQUEST_METHOD */
    const char* method = apr_pstrdup(r->pool, r->method);
    varlist = tpl_addVar("server_request_method", method, varlist);

    /* REQUEST_TIME */
    status = apr_ctime(ctime, r->request_time);
    varlist = tpl_addVar("server_request_time", ctime, varlist);

    /* QUERY_STRING */
    const char* queryString = apr_pstrdup(r->pool, r->parsed_uri.query);
    varlist = tpl_addVar("server_query_string", queryString, varlist);

    /* DOCUMENT_ROOT */
    const char* documentRoot = ap_document_root(r);
    varlist = tpl_addVar("server_document_root", documentRoot, varlist);

    /* HTTP_ACCEPT */
    const char* accept = apr_table_get(r->headers_in, "Accept");
    varlist = tpl_addVar("server_http_accept", accept, varlist);

    /* HTTP_ACCEPT_CHARSET */
    const char* acceptCharset = apr_table_get(r->headers_in, "Accept-Charset");
    varlist = tpl_addVar("server_http_accept_charset", acceptCharset, varlist);

    /* HTTP_ACCEPT_ENCODING */
    const char* acceptEncoding = apr_table_get(r->headers_in, "Accept-Encoding");
    varlist = tpl_addVar("server_http_accept_encoding", acceptEncoding, varlist);

    /* HTTP_ACCEPT_LANGUAGE */
    const char* acceptLanguage = apr_table_get(r->headers_in, "Accept-Language");
    varlist = tpl_addVar("server_http_accept_language", acceptLanguage, varlist);

    /* HTTP_CONNECTION */
    const char* acceptConnection = apr_table_get(r->headers_in, "Connection");
    varlist = tpl_addVar("server_http_connection", acceptConnection, varlist);

    /* HTTP_HOST */
    const char* httpHost = apr_table_get(r->headers_in, "Host");
    varlist = tpl_addVar("server_http_host", httpHost, varlist);

    /* HTTP_REFERER */
    const char* httpReferer = apr_table_get(r->headers_in, "Referer");
    varlist = tpl_addVar("server_http_referer", httpReferer, varlist);

    /* HTTP_USER_AGENT */
    const char* httpUserAgent = apr_table_get(r->headers_in, "User-Agent");
    varlist = tpl_addVar("server_http_user_agent", httpUserAgent, varlist);

    /* HTTPS */
    const char* scheme = apr_pstrdup(r->pool, r->parsed_uri.scheme);
    if (scheme) {
        if (!apr_strnatcmp(scheme, "https")) {
            varlist = tpl_addVar("server_https", "true", varlist);
        } else {
            varlist = tpl_addVar("server_https", "false", varlist);
        }
    } else {
        varlist = tpl_addVar("server_https", "false", varlist);
    }

    /* REMOTE_ADDR */
    status = apr_sockaddr_ip_get(&addr, r->connection->client_addr);
    if (status != APR_SUCCESS) {

        varlist = tpl_addVar("server_remote_addr", "Unable To Obtain", varlist);

    } else {

        varlist = tpl_addVar("server_remote_addr", addr, varlist);

    }

    /* REMOTE_HOST */
    const char* remoteHost = ap_get_remote_host(r->connection, r->per_dir_config,
            REMOTE_HOST, NULL);
    varlist = tpl_addVar("server_remote_host", remoteHost, varlist);

    /* REMOTE_PORT */
    apr_port_t remotePort = r->connection->client_addr->port;
    varlist = tpl_addVar("server_remote_port", apr_itoa(r->pool, remotePort), varlist);

    /* REMOTE_USER */
    const char* remoteUser = apr_pstrdup(r->pool, r->user);
    varlist = tpl_addVar("server_remote_user", remoteUser, varlist);

    /* SCRIPT_FILENAME */
    varlist = tpl_addVar("server_script_filename", self, varlist);

    /* SERVER_ADMIN */
    const char* serverAdmin = apr_pstrdup(r->pool, r->server->server_admin);
    varlist = tpl_addVar("server_server_admin", serverAdmin, varlist);

    /* SERVER_PORT */
    apr_port_t serverPort = r->server->port;
    varlist = tpl_addVar("server_server_port", apr_itoa(r->pool, serverPort), varlist);

    /* SERVER_SIGNATURE */
    const char* serverSignature = ap_psignature("", r);
    varlist = tpl_addVar("server_server_signature", serverSignature, varlist);

    /* REQUEST_URI */
    const char* requestURI = apr_pstrdup(r->pool, r->uri);
    varlist = tpl_addVar("server_request_uri", requestURI, varlist);

    /* AUTH_DIGEST */
    const char* authDigest = apr_table_get(r->headers_in, "Authorization");
    varlist = tpl_addVar("server_auth_digest", authDigest, varlist);

    /* AUTH_USER */
    const char* authUser = apr_pstrdup(r->pool, r->parsed_uri.user);
    varlist = tpl_addVar("server_auth_user", authUser, varlist);

    /* AUTH_PW */
    const char* authPassword = apr_pstrdup(r->pool, r->parsed_uri.password);
    varlist = tpl_addVar("server_auth_password", authPassword, varlist);

    /* AUTH_TYPE */
    const char* authType = ap_auth_type(r);
    varlist = tpl_addVar("server_auth_type", authType, varlist);

    /* PATH_INFO */
    const char* pathInfo = apr_pstrdup(r->pool, r->path_info);
    varlist = tpl_addVar("server_path_info", pathInfo, varlist);

    return varlist;


}

tpl_varlist* getSessionGlobal(request_rec* r, tpl_varlist* varlist) {

    dartSession theSession;
    tpl_loop *loop = NULL;

    int sessionCallback(void* r, const char* key, const char* value) {

        loop = tpl_addVarList(loop, tpl_addLoopVar(
                "key", key, "val", value));
        return 1;
    }

    /* See if we have a session, assume not */
    varlist = tpl_addVar("session_active", "false", varlist);
    int haveSession = hasSession(r);
    if (haveSession == -1) {

        logError("buildApacheClass -Failed to load mod_session, check your vhost", r->pool, 0);

    } else if (haveSession == TRUE) {

        /* Get the session */
        sessionStart(r, &theSession);
        /* Load the session variables */
        if (apr_is_empty_table(theSession.modSession->entries) == FALSE) {
            apr_table_do(sessionCallback, r, theSession.modSession->entries, NULL);
            return tpl_addLoop(varlist, "session_map", loop);
        }
        return varlist;
    }

    return varlist;

}

apr_file_t* buildApacheClass(const char* templatePath, const char* cachePath, request_rec* r) {

    tpl_varlist* varList = NULL;
    apr_file_t* scriptFile;
    apr_status_t status;
    FILE* fp;
    int tmplStatus = -1;
    char scriptFileTemplate[PATH_MAX];
    char* scriptFileTemplatePath;
    apr_size_t len;
    const char* scriptFileName;

    /* Build the apache environment */

    /* Version */
    varList = tpl_addVar("version", VERSION, NULL);

    /* SERVER super global */
    varList = getServerGlobal(r, varList);

    /* GET global, just look for this */
    varList = getGetGlobal(r, varList);

    /* POST/FILES global */

    /* Check for a POST or PUT */
    if ((r->method_number == M_POST) || (r->method_number == M_PUT)) {

        /* Check the content type */
        const char* type = NULL;
        type = apr_table_get(r->headers_in, "Content-Type");

        if (type != NULL) {

            /* If default(x-www-form-urlencoded) get normal post globals */
            if (strncaseEQ(type, DEFAULT_ENCTYPE, DEFAULT_ENCTYPE_LENGTH)) {
                varList = getPostGlobal(r, varList);
            } else {
                /* Get multipart globals, also builds the FILES global; */
                varList = getPostGlobalMultiPart(r, varList, cachePath);
            }
        }
    }

    /* COOKIES global */
    varList = getCookiesGlobal(r, varList);

    /* Request Headers  */
    varList = getRequestHeaders(r, varList);

    /* SESSION global */
    varList = getSessionGlobal(r, varList);

    /* Create the template file output file, get its name and close it. */
    len = sizeof (scriptFileTemplate);
    apr_cpystrn(scriptFileTemplate, cachePath, len);
    scriptFileTemplatePath = apr_pstrcat(r->pool, scriptFileTemplate, "XXXXXX", NULL);
    scriptFile = getTempFile(scriptFileTemplatePath, r->pool);
    if (scriptFile == NULL) return NULL;
    status = apr_file_name_get(&scriptFileName, scriptFile);
    status = apr_file_close(scriptFile);
    if (status != APR_SUCCESS) {
        logError("buildApacheClass - Failed to create apache template file", r->pool, status);
        return NULL;
    }

    /* Template needs normal files, not APR types, create the template file  */
    fp = fopen(scriptFileName, "a");
    tmplStatus = tpl_write(templatePath, varList, fp);
    if (tmplStatus) {

        logError("mod_dart - buildApacheClass - TMPL_write failed", r->pool, 0);
        return NULL;
    }
    fclose(fp);

    /* Free the var list */
    tpl_free(varList);

    return scriptFile;

}

char* parseBuffer(char* input, request_rec* r) {

    json_t *root;
    json_error_t error;
    json_t *l1Value;
    json_t *l2Value;
    const char* l1Key;
    const char* l2Key;
    int sessionActive = FALSE;


    /* Check for a sentinel, if none just return the buffer,
     * we have a VM script parse error,
     * a separate line ending in \n or a long line.
     */
    if (strstr(input, SENTINEL) == NULL) return input;

    /* Otherwise we have the control buffer on its own, 
     * split and get it. */
    char* controlBuffer = (strstr(input, SENTINEL) + SENTINEL_LENGTH);

    /* Parse the JSON encoded control buffer */
    root = json_loads(controlBuffer, 0, &error);
    if (root) {

        json_object_foreach(root, l1Key, l1Value) {

            switch (getCBSwitchInt(l1Key)) {

                case CB_INT_START:
                {
                    /* Start processing hook */
                    json_decref(l1Value);
                    break;
                }
                case CB_INT_HEADERS:
                {

                    json_object_foreach(l1Value, l2Key, l2Value) {

                        switch (getHSwitchInt(l2Key)) {

                            case H_INT_CONTENT_TYPE:
                            {
                                ap_set_content_type(r, apr_pstrdup(r->pool, json_string_value(l2Value)));
                                json_decref(l2Value);
                                break;
                            }

                            case H_INT_NORMAL:
                            {
                                apr_table_set(r->headers_out, l2Key, apr_pstrdup(r->pool, json_string_value(l2Value)));
                                json_decref(l2Value);
                                break;
                            }

                            default:
                            {
                            }

                        }

                    }

                    json_decref(l1Value);
                    break;
                }

                case CB_INT_END:
                {
                    /* End processing hook */
                    json_decref(l1Value);
                    break;
                }

                case CB_INT_SESSION_ACTIVE:
                {

                    dartSession theSession;

                    if (json_is_true(l1Value) == TRUE) {
                        sessionActive = TRUE;
                    } else {
                        sessionActive = FALSE;
                        int status = sessionStart(r, &theSession);
                        if (status == FALSE) break;
                        sessionDestroy(r, &theSession);
                    }

                    json_decref(l1Value);
                    break;

                }

                case CB_INT_SESSION:
                {

                    if (sessionActive == TRUE) {

                        dartSession theSession;

                        /* Get a session now we are active */
                        int status = sessionStart(r, &theSession);
                        if (status == FALSE) break;

                        /* Clear the entries table */
                        apr_table_clear(theSession.modSession->entries);

                        /* Re-instate the entries table */
                        json_object_foreach(l1Value, l2Key, l2Value) {

                            sessionSet(r, &theSession, l2Key, json_string_value(l2Value));
                            json_decref(l2Value);
                        }

                        sessionSave(r, &theSession, TRUE);
                    }

                    json_decref(l1Value);
                    break;
                }

                case CB_INT_SEND_HEADER:
                {

                    json_object_foreach(l1Value, l2Key, l2Value) {
                        apr_table_set(r->headers_out, l2Key, apr_pstrdup(r->pool, json_string_value(l2Value)));
                        json_decref(l2Value);
                    }

                    json_decref(l1Value);
                    break;
                }

                case CB_INT_STATUS:
                {
                    
                    r->status = json_integer_value(l1Value);
                    json_decref(l1Value);
                    break;
                    
                }
                default:
                {
                    return "mod-dart ERROR!! - Control Buffer Corrupt - Unknown Object Name";
                }
            }


        }


    } else {
        return "mod-dart ERROR!! - Control Buffer Corrupt - Cannot JSON Decode it - Check your Apache TPL file";
    }

    /* Clean up and return the now empty client buffer */
    json_decref(root);
    return input = "\n\0";


}

