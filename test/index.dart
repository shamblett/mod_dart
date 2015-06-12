
import 'dart:io';
import 'dart:convert';

void main() {

  // Get Apache
  Apache myAp = new Apache();
  myAp.writeOutput("<h1>I am now a real class!</h1>");

  // Set a header
  myAp.setHeader(Apache.CONTENT_LANGUAGE, 'de');
 
  // Dump the environment depending on format selection
  if ( myAp.Get['format'] == 'json' ) {
	myAp.setHeader(Apache.CONTENT_TYPE, "application/json");
        myAp.dumpEnvironmentJSON();
  } else {
  	myAp.setHeader(Apache.CONTENT_TYPE, "text/html");
	myAp.dumpEnvironment();
  }

  // Flush buffers an exit
  myAp.flushBuffers();
	  
}
