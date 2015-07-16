##
## Package : mod_dart
## Author : S. Hamblett <steve.hamblett@linux.com>
## Date   : 26/05/2015
## Copyright :  S.Hamblett 2015
## License : GPL V3, see the LICENSE file for details
##


# setup
current_dir = $(shell pwd)
template_path = /var/www/html/template
SRC=$(current_dir)/src

# third party
CTMPL=$(current_dir)/ctemplate-1.0/
JANS=$(current_dir)/jansson/
POPEN=$(current_dir)/popen-noshell
APREQ=$(current_dir)/apreq

# platform
DISTRO=$(shell lsb_release -si)
ifneq (,$(findstring Ubuntu,$(DISTRO)))
    APXS=apxs2
    UBUNTU=-Wc,-Wno-format-security
else
    APXS=apxs
endif


#   the default target
all: mod_dart

# main module target 
.PHONY: mod_dart
mod_dart: 
	
	$(APXS) -a -c -Wc,-Wall $(UBUNTU) -Wi, $(SRC)/mod_dart.c $(SRC)/template.c $(SRC)/utils.c $(SRC)/error.c $(SRC)/session.c $(SRC)/apache.c \
	    -I$(CTMPL) $(CTMPL)/ctemplate.c \
	    -I$(JANS) $(JANS)/dump.c $(JANS)/hashtable.c $(JANS)/hashtable_seed.c $(JANS)/load.c \
	    $(JANS)/memory.c $(JANS)/pack_unpack.c $(JANS)/strbuffer.c $(JANS)/strconv.c $(JANS)/utf.c \
	    $(JANS)/value.c $(JANS)/error.c \
	    -I$(POPEN) $(POPEN)/popen_noshell.c \
	    -I$(APREQ) $(APREQ)/apache_cookie.c $(APREQ)/apache_multipart_buffer.c $(APREQ)/apache_request.c  
	
	cp apacheTemplate/apacheTemplate.tpl $(template_path)


#   cleanup
clean:
	rm -f .libs/* rm *.lo rm *.la rm *.slo
