# mod_dart

An Apache module for the Dart language.

## Introduction

This module provides an Apache class that allows Dart code to be executed in a similar manner to PHP, i.e Dart code
can be called directly from the URL, e.g ```http://myserver.com/index.dart``` and its output
sent back to the client. In fact, the module is modelled on PHP in that the set of PHP super-globals is 
provided, this allows PHP constructs such as $_GET to be modelled as Apache.Get for module users, 
some examples, Apache.Get["animal"], Apache.Post["car"]. 

The Apache class also allows interaction with Apache so that headers etc. can be set by the Dart code 
before any output is returned to the user. 

As a heads up, have a quick look at the example [index.dart](test/index.dart) in the test directory 
for a flavour of what is provided, or go [here]((http://moddart.no-ip.net/index.dart?animal=frog) for a quick
but unexciting live demo.

The module is configured as a standard Apache module using conf directives, see [dart.conf](test/dart.conf) 
as an example, more on this later. Notice that the ```DartExePath``` directive sets the path to the 
Dart executable to use, this is just a standard Dart executable as found in the any Dart SDK, 
no special builds of Dart are required to use this module.

OK, now you have the gist, lets get into some specifics.

## Installation

### Build

The module itself is a standard built apache module using apxs, the project contains a  Netbeans project 
folder for Netbeans users but this is not needed to build the module. Prebuilt modules(64 bit) for 
Centos/Fedora/Rhel and Ubuntu 14.04 can be found in the 'modules' directory. For purpose of example 
I'll assume the build will take place on a Centos/Fedora/RHEL platform. 

Install httpd and its associated devel package.

In the top level directory of the project type ```make```

The module when built will appear in the ```.libs``` directory as mod_dart.so. Note that on my build 
machines the builds are clean, you should get no warnings.

When built copy the module to the httpd modules directory, ```/usr/lib64/httpd/modules/``` on my machine.

Note:: Linux only at the mo, however the code uses  standard C functions, Apaches 'ap' functions and
Apaches 'apr' portable runtime library so should be able to be ported to say Windows without to much
trouble(*disclaimer*::I've not tried it, it may be hard!).

### Configuration

The module is configured as a standard Apache module with its own directives, firstly we have to add a 
handler for Dart :-

```<Location /> AddHandler dart .dart </Location>```

with the '/' of location being your document root as a default.

Secondly we have to set the module specific directives :-

1. ```DartExePath``` - mandatory, the path to the Dart executable to use, must be executable by the webserver
2. ```CachePath ``` - mandatory, a cache directory path, must be read/writeable by the webserver and in
the document root
3. ```TemplatePath``` - mandatory, the path to the Apache class template to use, must be in the document root, 
more on this later.
4. ```PackageRoot``` - optional, the package root for the VM, passed as the --package-root option, 
if not set this will default to ServerRoot or your document root if set, must be in your document root.


If you are using a virtual host the AddHandler directive goes in the 'Directory' config clause, the other
directives go in the virtual host declaration, this allows different virtual hosts
to have different Dart VM's cache paths etc. 


See [dart.conf](test/dart.conf) for an example, note for pure directory specifications like the cache path
the trailing '/' *must* be supplied.


There is also an associated module config file in the test directory, [00-dart.conf](test/00-dart.conf). 
this should be copied to your apache modules conf directory.

### Testing

Copy the file ```index.dart``` from the test directory to your server root or specified Location, 
in your browser(assuming localhost) type the following URL:- 
```
http://localhost/index.dart/somestuff?animal=budgie&car=honda&name=fred&format=html
```

If all is well you should see an Apache environment dump in HTML format, change the 'format' parameter 
to 'json' and you get the same but in JSON format. See the file [mod_dart.html](test/mod_dart.html) in the 
test directory for what you should see. Please check the apache error log for any errors, especially if
you get a 500 error(see Error Handling below).

You can also use curl commands for testing of course, an example curl command is shown in the 
[curl.md](test/curl.md) document in the test directory, this shows how to set cookie and post data.

There's also the live demo, see above.

OK, so how does it all work?

## How it works

In essence there are two ways to do this IMHO, there's the Dart VM fully integrated into an Apache module approach
as in Sam McColl's original [mod_dart](https://github.com/sam-mccall/mod_dart) or a way of invoking the Dart VM from inside 
an Apache module as a standalone entity. Lets call the first approach 'embedded', the Dart VM is compiled into the Apache module so 
file which also acts as a native extension. Lets call the second approach 'controlled' where the Apache module invokes and
controls the VM but the VM stays as a standalone entity.

This implementation of mod_dart uses the the controlled approach. I did attempt to get the embedded approach working
but failed in the end, on 64 bit platforms you need to compile the whole VM with -fPIC(position independent code) to
enable it to be embedded in a so file. This entails changing various gyp/gypi files in the Dart build hierarchy to achieve
this, then there's the 3rd party code to worry about, this all became too complex and very brittle, the VM is definitely
built to be static! Note for interested parties I can supply a fuller description of what I did here and how far I got, 
please mail me.

Basically the controlled approach leverages the power of the VM to run complete valid Dart scripts and to send its output to
standard out via print statements, so we do this :-

1. Get the incoming script and copy it to a temp file, this must be a valid Dart script with a main() function.

2. Load a templated Dart class(Apache), see [apacheTemplate.tpl](apacheTemplate/apacheTemplate.tpl) for an example, note that
   this is a valid Dart script albeit with TMPL markup where the template will be filled in.

3. Get the Apache runtime environment and set this in the templated class, we now have a static Dart class containing
   the Apache environment.

4. Append the templated class to the end of the temp script file creating a Dart script file that now has access to
   the Apache class.

5. Invoke the VM with this script and any other options by using 'popen'.

6. Collect the output of the popened VM and return it to the caller.

OK, so how do we we do things like set headers, ie instructions to Apache rather than raw output? We make
the output a combination of data and control buffers, the data buffer is split off and returned to the user,
the control buffer is parsed first with any commands specified applied. The control buffer is actually encoded
in JSON format. Please inspect the index.dart file and the template itself to get a feel of how all this works together
along with the actual code of course.

There are pro's and con's to both approaches, the embedded approach will always be faster(possibly a lot) than the 
controlled approach, creating temp files and launching shell processes will always be slower than a truly embedded VM, 
however the embedded approach needs both a special Dart VM build and a native extension imported into your Dart code, this is
not simple and becomes very platform specific, the controlled approach avoids all this at a speed cost. There's also
testing to to consider, in theory you can test scripts written for the controlled approach without using mod_dart, you
can just edit your template adding the data you wish and just do an 'import 'package:apache/apache.dart', this however does 
need an element of test harness support.


## Creating your Dart scripts

Your scripts can be any valid Dart scripts with a main() function, they also *must* import dart:convert, 
these are the only restrictions mod-dart imposes.The methodology of how the script generates output is akin to the
PHP 'ob' methods, i.e. output buffers's, whereby any script output is buffered and only written back to the client when 
the buffer is flushed. In PHP this is optional, you can write directly back to Apache at any point, in mod_dart it
isn't, everything is buffered, a quick example :-
```
import 'dart:io';
import 'dart:convert';

void main() {

  // Set a header
  Apache.setHeader(Apache.CONTENT_TYPE, "text/html");

  // Write some output
  Apache.writeOutput('<h1>Hello from MOD_DART!');
  
  // Flush the buffers
  Apache.flushBuffers();

}
```
You can call Apache.writeOutput as many times as you wish, when Apache.flushBuffers is called your output
(plus the built control block) is sent back to Apache. You could of course carry on processing in the VM after 
this but there's no real point. Unlike a normal server side VM application that's built to stay alive your mod_dart
scripts should be built in the opposite way, i.e. to perform one function and die(a la PHP). You can use timer routines
if you wish but why would you want to do this, you could also use normal 'print' statements but this would corrupt(preceed)
any output generated by Apache.writeOutput. You can use any valid Dart construct you wish because its valid Dart and I can't
stop this but please be sensible here.

Note that Dart scripts can be called from anywhere valid, not just the URL, so you can use them as a form actions,
e.g 
```
<form action="demo_form.dart" method="get" target="_blank">
```

## Error Handling

There are two main levels of error handling, failures in mod_dart and errors in your script(this includes the template of 
course if you've edited it). 

Failures in mod_dart that are captured will result in a 500 code being returned and a line being
output in the Apache error log as to the nature of the failure. These are normally due to misconfiguration of the module,
wrong VM perms, wrong cache perms, invalid paths etc. At the moment you need to find the failure string in the mod_dart code
to see whats happening(I will improve this). Failures in mod_dart that are not captured e.g. sigsev's will result in a 
'no data received' error in the browser, this shouldn't happen but if it does only gdb will be able to help if there is
no output in the error log.

Script failures are reported back to the browser as raw VM output(the popened shell also picks up std error as well as
std output), example :-
```
Unhandled exception:
No top-level getter 'Apachlle' declared.

NoSuchMethodError: method not found: 'Apachlle'
Receiver: top-level
Arguments: [...]
#0      NoSuchMethodError._throwNew (dart:core-patch/errors_patch.dart:176)
#1      main (file:///var/www/html/cache/CYnFzZ:8:3)
#2      _startIsolate.<anonymous closure> (dart:isolate-patch/isolate_patch.dart:255)
#3      _RawReceivePortImpl._handleMessage (dart:isolate-patch/isolate_patch.dart:142)

```
Use the'show source' command of your browser to see this better. Just fix as normal.

## Why and Where

### Why

Why do this? Well one thing I like about PHP(and there's plenty not too) is I can just do this anywhere
to get a posted variable '$_POST["name]", its easy(I'm a coder, thus lazy) , Iv'e always wanted to do this in Dart 
scripts, also
if your not writing IOT stuff where you are embedding the VM and it needs its own light weight web server or not writing
Single Page Applications(not everything is an SPA, there are some web sites with really lots of pages!) then you may
need the heavy lift that Apache can bring, need to redirect depending on URL, use htaccess/mod_rewrite, need to authenticate
against LDAP, use mod_auth_ldap, want to perform redirection based on country, mod_geoip, want to do something wizzy,
mod_dosomethingwizzy, you get the picture. All with the security inherent in the Dart VM. 
 
### Where

Well, firstly it needs to be finished, Sessions and File superglobals need to be added, far more control
is needed to interact with Apache and load testing needs doing, then there's debugging support, test harnesses
etc. snapshots?, lots to do in fact. Also a more 'real' application needs writing that imports lots of packages to get
a real feel for this. See the [omissions](test/omissions.md) document in the test directory for what is/isn't supported.







