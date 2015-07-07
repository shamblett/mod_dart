
import 'dart:io';
import 'dart:convert';

void main() {

  // Get Apache
  Apache myAp = new Apache();

  // Set a header
  myAp.setHeader(Apache.CONTENT_LANGUAGE, 'us');

  // Session tests
  if ( myAp.Request['sesscontrol'] == 'start' ) {
	myAp.startSession();
	myAp.Session['Name'] = 'Billy';
	myAp.Session['Sex'] = 'Male';
  }

  if ( myAp.Request['sesscontrol'] == 'change' ) {
	myAp.startSession();
	myAp.Session['Name'] = 'Fred';
	myAp.Session.remove('Sex');
  }

  if ( myAp.Request['sesscontrol'] == 'end' ) {
	myAp.startSession();
	myAp.endSession();
  }
 
  if ( myAp.Request['sesscontrol'] == 'open' ) {
	myAp.startSession();
  }
	
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
