
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

  // Delete any uploaded files
  if ( myAp.Request.containsKey('deleteFiles') ) {

    myAp.Files.forEach((String key, Map value){

        if ( value.containsKey('tmp_name') ) {
             var myFile = new File(value['tmp_name']);
             myFile.deleteSync();
        }
    });
  }

  // Send Header if asked 
  if ( myAp.Request.containsKey('sendHeader') ) {

    myAp.header("Location", "http://www.google.com");
  }

  // Die if asked
  if ( myAp.Request.containsKey('die') ) {

     myAp.die("Test of the die function");

  }

  // Test print_r
  List myList = [ 'first', 'second', 'third'];
  int myInt = 10;
  double myDouble = 1.5678;
  if ( myAp.Request.containsKey('printr') ) {

     myAp.print_r(myList,"myList is : ");
     myAp.print_r(myInt);
     myAp.writeOutput("${myAp.print_r(myDouble, 'The Double is : ', true)}");
  }

  // Flush buffers and exit
  myAp.flushBuffers();
	  
}
