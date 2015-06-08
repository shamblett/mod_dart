import 'dart:io';
import 'dart:convert';

void main() {

  // Set a header
  Apache.setHeader(Apache.CONTENT_LANGUAGE, 'de');

  // Dump the environment depending on format selection
  if ( Apache.Get['format'] == 'json' ) {
        Apache.setHeader(Apache.CONTENT_TYPE, "application/json");
        Apache.dumpEnvironmentJSON();
  } else {
        Apache.setHeader(Apache.CONTENT_TYPE, "text/html");
        Apache.dumpEnvironment();
  }

  // Flush buffers an exit
  Apache.flushBuffers();

	  
}
