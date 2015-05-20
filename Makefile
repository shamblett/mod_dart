   ##
##  Makefile -- Build procedure for the mod_dart Apache module
##  S. Hamblett May 2015
##

builddir=.
home=/home/steve


#   the used tools
APXS=apxs

#   the default target
all: dart

# main module target 
.PHONY: dart
dart: 
	
	$(APXS) -a -c -Wc,-Wall -Wi, mod_dart.c


#   cleanup
clean:
	rm -f .libs/* rm *.lo rm *.la rm *.slo
