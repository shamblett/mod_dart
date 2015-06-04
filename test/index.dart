
import 'dart:io';
import 'dart:convert';

void main() {

 
  if ( Apache.Get['format'] == 'json' ) {
	Apache.setHeader(Apache.CONTENT_TYPE, "application/json");
	Apache.dumpEnvironmentJSON();
  } else {
  	Apache.setHeader(Apache.CONTENT_TYPE, "text/html");
	Apache.dumpEnvironment();
  }

  Apache.flushBuffers();
	  
}
