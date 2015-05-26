##
##  Makefile -- Build procedure for the mod_dart Apache module
##  Copyright S. Hamblett May 2015
##

# setup
current_dir = $(shell pwd)
template_path = /var/www/html/template

# third party
CTMPL=$(current_dir)/ctemplate-1.0/

#   the used tools
APXS=apxs

#   the default target
all: dart

# main module target 
.PHONY: dart
dart: 
	
	$(APXS) -a -c -Wc,-Wall -Wi,  mod_dart.c template.c utils.c error.c apache.c \
	    -I$(CTMPL) $(CTMPL)/ctemplate.c
	cp apacheTemplate/apacheTemplate.tpl $(template_path)


#   cleanup
clean:
	rm -f .libs/* rm *.lo rm *.la rm *.slo
