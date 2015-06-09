# mod_dart

An Apache module for the Dart language.

## Introduction

This module provides an Apache class that allows Dart code to be executed in a similar manner to PHP, i.e Dart code
can be called directly from the URL, e.g ```http://myserver.com/index.dart``` and its output
sent back to the client. In fact, the module is modelled on PHP in that the set of PHP super-globals is 
provided, this allows PHP constructs such as $_GET to be modelled as Apache.Get for module users, 
some examples, Apache.GET["animal"], Apache.Post["car"]. Please see the [omissions.md](test/omissions.md) 
document in the test directory for exactly what is provided. 

The Apache class also allows interaction with Apache so that headers etc. can be set by the Dart code 
before any output is returned to the user. 

As a heads up, have a quick look at the example [index.dart](test/index.dart) in the test directory 
for a flavour of what is provided.

The module is configured as a standard Apache module using conf directives, see [dart.conf](test/dart.conf) 
as an example, more on this later. Notice that the ```DartExePath``` directive sets the path to the 
Dart executable to use, this is just a standard Dart executable as found in the any Dart SDK, 
no special builds of Dart are required to use this module.

OK, now you have the gist, lets get into some specifics.

## Installation

### Build

The module itself is a standard built apache module using apxs, the project contains a  Netbeans project 
folder for Netbeans users but this is not needed to build the module. Prebuilt modules(64 bit) for 
Centos/Fedora/Rhel and Ubuntu 14.04 can be found in the ```modules``` directory. For purpose of example 
I'll assume the build will take place on a Centos/Fedora/RHEL platform. 

Install httpd and its associated devel package.

In the top level directory of the project type ```make```

The module when built will appear in the ```.libs``` directory as mod_dart.so. Note that on my build 
machines the builds are clean, you should get no warnings.

When built copy the module to the httpd modules directory, ```/usr/lib64/httpd/modules/``` on my machine.

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


If you are using a virtual host the AddHandler directive goes in the 'Directory' config clause.


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
you get a 500 error.

You can also use curl commands for testing of course, an example curl command is shown in the 
[curl.md](test/curl.md) document in the test directory, this shows how to set cookie and post data.

To boot I've installed this one of my test boxes, go [here](http://moddart.no-ip.net/index.dart?animal=frog) for
the demo, not very exciting though!

OK, so how does it all work?

## How it works

In essence there are two ways to do this IMHO, there's the Dart VM fully integrated into an Apache module approach
as in Sam McColl's original [mod_dart](https://github.com/sam-mccall/mod_dart) or a way of invoking the Dart VM from inside 
an Apache module as a standalone entity. Lets call the first approach 'embedded', the Dart VM is compiled into the Apache module so 
file which also acts as a native extension. Lets call the second approach 'controlled' where the Apache module invokes and
controls the VM but the VM stays as a standalone entity.

This implementation of mod_dart uses the the controlled approach. I did attempt to get the embedded approach working
but failed in the end, on 64 bit platforms you need to compile the whole VM with -fPIC(position independent code) to
enable it to be embedded in a so file. This entails changing various gypi files in the Dart build hierarchy to achieve
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

The target Dart script can be any valid Dart script but *must* import dart:convert, this is the only restriction
mod-dart imposes. 



