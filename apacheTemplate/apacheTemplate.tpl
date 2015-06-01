
//
//--------------------------------------------------------------------
//
// Start of Apache class script, auto appended by mod_dart
// Version <TMPL_VAR name = "version"> S.Hamblett 2015
//

class Apache{ 

    // Global settings
    
    static final String version =  '<TMPL_VAR name = "version">';
        
    // SERVER superglobal
    
    static final Map Server = { 'SELF' : '<TMPL_VAR name = "server_self">',
                                'SERVER_ADDR' : '<TMPL_VAR name = "server_addr">',
                                'SERVER_NAME' : '<TMPL_VAR name = "server_name">',
                                'SERVER_SOFTWARE' : '<TMPL_VAR name = "server_software">',
                                'SERVER_PROTOCOL' : '<TMPL_VAR name = "server_protocol">',
                                'REQUEST_METHOD' : '<TMPL_VAR name = "server_request_method">',
                                'REQUEST_TIME' : '<TMPL_VAR name = "server_request_time">',
                                'QUERY_STRING' : '<TMPL_VAR name = "server_query_string">',
                                'DOCUMENT_ROOT' : '<TMPL_VAR name = "server_document_root">',
                                'HTTP_ACCEPT' : '<TMPL_VAR name = "server_http_accept">',
                                'HTTP_ACCEPT_CHARSET' : '<TMPL_VAR name = "server_http_accept_charset">',
                                'HTTP_ACCEPT_ENCODING' : '<TMPL_VAR name = "server_http_accept_encoding">',
                                'HTTP_ACCEPT_LANGUAGE' : '<TMPL_VAR name = "server_http_accept_language">',
                                'HTTP_CONNECTION' : '<TMPL_VAR name = "server_http_connection">',
                                'HTTP_HOST' : '<TMPL_VAR name = "server_http_host">',     
                                'HTTP_REFERER' : '<TMPL_VAR name = "server_http_referer">', 
                                'HTTP_USER_AGENT' : '<TMPL_VAR name = "server_http_user_agent">', 
                                'HTTPS' : '<TMPL_VAR name = "server_https">',     
                                'REMOTE_ADDR' : '<TMPL_VAR name = "server_remote_addr">',  
                                'REMOTE_HOST' : '<TMPL_VAR name = "server_remote_host">',     
                                'REMOTE_PORT' : '<TMPL_VAR name = "server_remote_port">', 
                                'REMOTE_USER' : '<TMPL_VAR name = "server_remote_user">', 
                                'SCRIPT_FILENAME' : '<TMPL_VAR name = "server_script_filename">', 
                                'SERVER_ADMIN' : '<TMPL_VAR name = "server_server_admin">',  
                                'SERVER_PORT' : '<TMPL_VAR name = "server_server_port">', 
                                'SERVER_SIGNATURE' : '<TMPL_VAR name = "server_server_signature">',     
                                'REQUEST_URI' : '<TMPL_VAR name = "server_request_uri">', 
                                'AUTH_DIGEST' : '<TMPL_VAR name = "server_auth_digest">', 
                                'AUTH_USER' : '<TMPL_VAR name = "server_auth_user">', 
                                'AUTH_PW' : '<TMPL_VAR name = "server_auth_password">', 
                                'AUTH_TYPE' : '<TMPL_VAR name = "server_auth_type">', 
                                'PATH_INFO' : '<TMPL_VAR name = "server_path_info">'   
                                    
                                };
        
    // Functions
    
    // The output buffer
    static String _outputBuffer = "";
    
    // Write output to the output buffer
    static void writeOutput(String output) {
    
        _outputBuffer = _outputBuffer + output;
    
    }
    
    // Flush the output buffer back to apache
    static void flushOutput() {
    
        print(_outputBuffer);
    
    }
    
    // Clear the output buffer without flushing
    static void clearOutput() {
    
        _outputBuffer = "";
    }
    
    // Dump the apache environment
    static void dumpEnvironment() {
    
        writeOutput('<h1><u> Apache environment supplied by mod_dart.</u></h1>');
        writeOutput('<h2><u> General </u></h2>');
    
        // Version     
        writeOutput('<h3> Mod_Dart Version : ${version} </h3>');
        
        // SERVER
        writeOutput('<h2><u> Server </u></h2>');
        writeOutput('<h3> SELF : ${Server["SELF"]} </h3>');
        writeOutput('<h3> SERVER_ADDR : ${Server["SERVER_ADDR"]} </h3>');
        writeOutput('<h3> SERVER_NAME : ${Server["SERVER_NAME"]} </h3>');
        writeOutput('<h3> SERVER_SOFTWARE : ${Server["SERVER_SOFTWARE"]} </h3>');
        writeOutput('<h3> SERVER_PROTOCOL : ${Server["SERVER_PROTOCOL"]} </h3>');
        writeOutput('<h3> REQUEST_METHOD : ${Server["REQUEST_METHOD"]}  </h3>');
        writeOutput('<h3> REQUEST_TIME : ${Server["REQUEST_TIME"]}  </h3>');
        writeOutput('<h3> QUERY_STRING : ${Server["QUERY_STRING"]}  </h3>');
        writeOutput('<h3> DOCUMENT_ROOT : ${Server["DOCUMENT_ROOT"]}  </h3>');
        writeOutput('<h3> HTTP_ACCEPT : ${Server["HTTP_ACCEPT"]}  </h3>');
        writeOutput('<h3> HTTP_ACCEPT_CHARSET : ${Server["HTTP_ACCEPT_CHARSET"]}  </h3>');
        writeOutput('<h3> HTTP_ACCEPT_ENCODING : ${Server["HTTP_ACCEPT_ENCODING"]}  </h3>');
        writeOutput('<h3> HTTP_ACCEPT_LANGUAGE : ${Server["HTTP_ACCEPT_LANGUAGE"]}  </h3>');
        writeOutput('<h3> HTTP_CONNECTION : ${Server["HTTP_CONNECTION"]}  </h3>');
        writeOutput('<h3> HTTP_HOST : ${Server["HTTP_HOST"]}  </h3>'); 
        writeOutput('<h3> HTTP_REFERER : ${Server["HTTP_REFERER"]}  </h3>'); 
        writeOutput('<h3> HTTP_USER_AGENT : ${Server["HTTP_USER_AGENT"]}  </h3>'); 
        writeOutput('<h3> HTTPS : ${Server["HTTPS"]}  </h3>');
        writeOutput('<h3> REMOTE_ADDR : ${Server["REMOTE_ADDR"]}  </h3>');
        writeOutput('<h3> REMOTE_HOST : ${Server["REMOTE_HOST"]}  </h3>');
        writeOutput('<h3> REMOTE_PORT : ${Server["REMOTE_PORT"]}  </h3>');
        writeOutput('<h3> REMOTE_USER : ${Server["REMOTE_USER"]}  </h3>');
        writeOutput('<h3> SCRIPT_FILENAME : ${Server["SCRIPT_FILENAME"]}  </h3>');
        writeOutput('<h3> SERVER_ADMIN : ${Server["SERVER_ADMIN"]}  </h3>');
        writeOutput('<h3> SERVER_PORT : ${Server["SERVER_PORT"]}  </h3>');
        writeOutput('<h3> SERVER_SIGNATURE : ${Server["SERVER_SIGNATURE"]}  </h3>');
        writeOutput('<h3> REQUEST_URI : ${Server["REQUEST_URI"]}  </h3>');
        writeOutput('<h3> AUTH_DIGEST : ${Server["AUTH_DIGEST"]}  </h3>');
        writeOutput('<h3> AUTH_USER : ${Server["AUTH_USER"]}  </h3>');
        writeOutput('<h3> AUTH_PW : ${Server["AUTH_PW"]}  </h3>');
        writeOutput('<h3> AUTH_TYPE : ${Server["AUTH_TYPE"]}  </h3>');
        writeOutput('<h3> PATH_INFO : ${Server["PATH_INFO"]}  </h3>');
        
        // End
        writeOutput('<h3>------- End of Dump ------</h3>');
        
    }
        
}