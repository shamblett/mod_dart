
import 'dart:io';
import 'dart:convert';

void main() {

  // Get Apache
  Apache myAp = new Apache();

  // Set a header
  myAp.setHeader(Apache.CONTENT_LANGUAGE, 'en');
 
  // Dump the environment depending on format selection
  if ( myAp.Get['format'] == 'json' ) {
	myAp.setHeader(Apache.CONTENT_TYPE, "application/json");
        myAp.dumpEnvironmentJSON();
  } else {
  	myAp.setHeader(Apache.CONTENT_TYPE, "text/html");
	myAp.dumpEnvironment();
  }

  // Flush buffers and exit
  myAp.flushBuffers();
	  
}
