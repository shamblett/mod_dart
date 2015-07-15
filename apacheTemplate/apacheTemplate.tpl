
//
//--------------------------------------------------------------------
//
// Start of Apache class script, auto appended by mod_dart
// Version <TMPL_VAR name = "version"> S.Hamblett 2015
//

class Apache{ 

    // Global settings
    
    static final String version =  '<TMPL_VAR name = "version">';
    
    // Sentinel, must match the definition in mod-dart- do not edit
    static final String _sentinel = ":-:mod_dart_control:-:";    
    
    // Control buffer sections 
    static final String CB_HEADERS = 'Headers';
    static final String CB_SESSION = 'Session';
    static final String CB_SESSION_ACTIVE = 'Session_Active';
    static final String CB_END = 'End';
    
    // Response Header definitions, do NOT use the ones in HttpHeaders
    static final String CONTENT_TYPE = 'Content-Type';
    static final String ACCEPT = 'Accept';
    static final String ACCESS_CONTROL_ALLOW_ORIGIN = 'Access-Control-Allow-Origin';
    static final String ALLOW = 'Allow';
    static final String CACHE_CONTROL = 'Cache-Control';
    static final String CONNECTION = 'Connection';
    static final String CONTENT_DISPOSITION = 'Content-Disposition';
    static final String CONTENT_ENCODING = 'Content-Encoding';
    static final String CONTENT_LANGUAGE = 'Content-Language';
    static final String LOCATION = 'LOCATION';
    static final String PRAGMA = 'Pragma';
    static final String SET_COOKIE = 'Set-Cookie';
    static final String STATUS = 'Status';
    static final String TRANSFER_ENCODING = 'Transfer-Encoding';
    static final String WWW_AUTHENTICATE = 'WWW-Authenticate';
    
    // Super globals
    
    // SERVER superglobal
    final Map Server = { 'SELF' : '<TMPL_VAR name = "server_self">',
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
                                
   final Map Get = { <TMPL_LOOP name = "get_map">
                                    '<TMPL_VAR name = "key">' :'<TMPL_VAR name = "val">',
                            </TMPL_LOOP>   
                            };
                            
   final Map Post = { <TMPL_LOOP name = "post_map">
                                    '<TMPL_VAR name = "key">' :'<TMPL_VAR name = "val">',
                            </TMPL_LOOP>   
                            };
   final Map Cookie = { <TMPL_LOOP name = "cookie_map">
                                    '<TMPL_VAR name = "key">' :'<TMPL_VAR name = "val">',
                            </TMPL_LOOP>   
                            };
   Map Session = { <TMPL_LOOP name = "session_map">
                                    '<TMPL_VAR name = "key">' :'<TMPL_VAR name = "val">',
                            </TMPL_LOOP>   
                            };
    
    bool _sessionActive = false;
    
    final Map Files = { <TMPL_LOOP name = "file_map">
                                    '<TMPL_VAR name = "key">' :<TMPL_VAR name = "val">,
                            </TMPL_LOOP>   
                            };
                            
    Map _request = new Map();
   
    Map get Request {
    
        if ( _request.isEmpty ) {
            _request.addAll(Get);
            _request.addAll(Post);
            _request.addAll(Cookie);
        }
        return _request;
        
    }
    
    final Map _requestHeaders = { <TMPL_LOOP name = "request_header_map">
                                    '<TMPL_VAR name = "key">' : '<TMPL_VAR name = "val">',
                            </TMPL_LOOP>   
                            };
    
    
     Map<String, String> _responseHeaders = new Map<String, String>();       
        
    // Functions
    
    // HTTP protocol
    
    // Headers
    void setHeader(String name, String value) {
    
        _responseHeaders[name] = value;
    }
    
    void setCookie(var cookie) {
    
        _responseHeaders[SET_COOKIE] = cookie.toString();
    }
    
    Map requestHeaders() {
     
        return _requestHeaders;
    }   
    
    // Sessions
   
    void startSession() {
      _sessionActive = true;  
    }
    
    void endSession() { 
         Session.clear();
         _sessionActive = false;  
    }
    
    bool sessionActive() {
        return _sessionActive;
    }
    
    // Files
    
    bool moveUploadedFile(String tempname, String destination) {
    
        String name = path.basename(tempname);
        
        try {
            if ( name.startsWith('moddart-upload') ) {
                if  ( name.length == 20 ) {
                    var myFile = new File(tempname);
                    myFile.copySync(destination);
                    myFile.deleteSync(tempname);
                    return true;
                }
            }
        } catch (e) {
            return false;
        }             
        return false;   
    }
    
    // Class specific
        
    // The output buffer
    String _outputBuffer = "";
    // The control buffer
    String _controlBuffer = "";
   
    
    // Write output to the output buffer
    void writeOutput(String output) {
    
        _outputBuffer = _outputBuffer + output;
    
    }
    
    // Flush the buffers back to apache and exit the VM if
    // exit is true;
    void flushBuffers([bool exitVM = false]) {
    
        Map<String, Map> output = new Map<String, Map>();

        // Note,  ordering is important here
        output[CB_HEADERS] = _responseHeaders;
        output[CB_SESSION_ACTIVE] = _sessionActive;
        if ( _sessionActive ) output[CB_SESSION] = Session;
        output[CB_END] = null;

        _controlBuffer = _controlBuffer + JSON.encode(output);
        // Delimit the sentinel with a newline so it will never
        // span an fgets buffer in mod_dart.
        print(_outputBuffer + '\n' + _sentinel + _controlBuffer + '\n');
        if (exitVM) exit(0);
    }
    
    // Clear the output buffer without flushing
    void clearOutput() {
    
        _outputBuffer = "";
    }
    
    
    // Dump the apache environment
    void dumpEnvironment() {
    
        writeOutput('<h1><u> Apache environment supplied by mod_dart.</u></h1>');
        writeOutput('<h2><u> General </u></h2>');
    
        // Version     
        writeOutput('<h3> Mod_Dart Version : ${version} </h3>');
        
        // SERVER
        writeOutput('<h2><u> SERVER </u></h2>');
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
        
        // GET
        writeOutput('<h2><u>GET</u></h2>');
        Get.forEach((key, value) {
            writeOutput('<h3> ${key} : ${value} </h3>');
        });
        
        // POST
        writeOutput('<h2><u>POST</u></h2>');
        Post.forEach((key, value) {
            writeOutput('<h3> ${key} : ${value} </h3>');
        });
        
        // FILES
        writeOutput('<h2><u>FILES</u></h2>');
        Files.forEach((key, value) {
            writeOutput('<h3> ${key} : ${value.toString()} </h3>');
        });
        
        // COOKIES
        writeOutput('<h2><u>COOKIES</u></h2>');
        Cookie.forEach((key, value) {
            writeOutput('<h3> ${key} : ${value} </h3>');
        });
        
        //SESSION
        writeOutput('<h2><u>SESSION</u></h2>');
        writeOutput('<h3> sessionActive: ${_sessionActive.toString()} </h3>');
        if ( _sessionActive ) {
            Session.forEach((key, value) {
                writeOutput('<h3> ${key} : ${value.toString()} </h3>');       
            });
        }
        
        // REQUEST
        writeOutput('<h2><u>REQUEST</u></h2>');
        Request.forEach((key, value) {
            writeOutput('<h3> ${key} : ${value} </h3>');
        });
        
        // HEADERS
        writeOutput('<h2><u>HEADERS</u></h2>');
        requestHeaders().forEach((key, value) {
            writeOutput('<h3> ${key} : ${value} </h3>');
        });
        
        
        // End
        writeOutput('<h3>!------- End of Dump -------!</h3>');
        
    }
    
    // Dump the apache environment
    void dumpEnvironmentJSON() {
    
        writeOutput(JSON.encode(Server) + 
                    JSON.encode(Get) + 
                    JSON.encode(Post) + 
                    JSON.encode(Cookie) + 
                    JSON.encode(_sessionActive) +
                    JSON.encode(Session) + 
                    JSON.encode(Request) +
                    JSON.encode(requestHeaders()));
        
    }    
}