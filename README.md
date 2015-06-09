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

The module is configured as a standard Apache module using conf directives, see [dart.config](test/dart.conf) 
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

with the '/' of location being your server root as a default.

Secondly we have to set the module specific directives :-

1. ```DartExePath``` - mandatory, the path to the Dart executable to use.
2. ```CachePath ``` - mandatory, a cache directory path, must be read/writeable by the webserver, 
you can use /tmp if you wish.
3. ```TemplatePath``` - mandatory, the path to the Apache class template to use, more on this later.
4. ```PackageRoot``` - optional, the package root for the VM, passed as the --package-root option, 
if not set this will default to ServerRoot.

### Testing

Copy the file ```index.dart``` from the test directory to your server root or specified Location, 
in your browser(assuming localhost) type the following URL:- 
```http://localhost/index.dart/somestuff?animal=budgie&car=honda&name=fred&format=html```

If all is well you should see an Apache environment dump in HTML format, change the 'format' parameter 
to 'json' and you get the same but in JSON format.

OK, so how does it all work?

## How it works