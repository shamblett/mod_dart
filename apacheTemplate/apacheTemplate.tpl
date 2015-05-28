
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
                                'SERVER_NAME' : '<TMPL_VAR name = "server_name">'};
        
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
        writeOutput('<h3> Version : ${version} </h3>');
        
        // SERVER
        writeOutput('<h2><u> Server </u></h2>');
        writeOutput('<h3> SELF : ${Server["SELF"]} </h3>');
        writeOutput('<h3> SERVER_ADDR : ${Server["SERVER_ADDR"]} </h3>');
        writeOutput('<h3> SERVER_NAME : ${Server["SERVER_NAME"]} </h3>');
        
        // End
        writeOutput('<h3>------- End of Dump ------</h3>');
        
    }
        
}