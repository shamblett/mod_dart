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
	
	$(APXS) -S CC=g++ -o mod_dart.so -Wc,-Wall -Wi, -I -lstdc++  apache.c mod_dart.c


#   cleanup
clean:
	rm -f .libs/* 
