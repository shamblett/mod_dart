   ##
##  Makefile -- Build procedure for the mod_dart Apache module
##  S. Hamblett May 2015
##

builddir=.
home=/home/steve


#   the used tools
APXS=apxs

#   additional defines, includes and libraries
COPTS="-DNDEBUG -fPIC"

#   the default target
all: dart

# main module target 
.PHONY: dart
dart: 
	
	$(APXS) -S CC=g++ -c $(COPTS) -o mod_dart.so -Wc,-Wall -Wi, -lstdc++ mod_dart.c


#   cleanup
clean:
	rm -f .libs/* rm *.lo rm *.la rm *.slo
