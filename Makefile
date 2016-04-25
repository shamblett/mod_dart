##
## Package : mod_dart
## Author : S. Hamblett <steve.hamblett@linux.com>
## Date   : 26/05/2015
## Copyright :  S.Hamblett 2015
## License : GPL V3, see the LICENSE file for details
##


# setup
current_dir = $(shell pwd)
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

SRC_FILES = $(SRC)/mod_dart.c $(SRC)/template.c $(SRC)/utils.c $(SRC)/error.c \
            $(SRC)/session.c $(SRC)/apache.c
            
TEMPL_FILES = $(CTMPL)/ctemplate.c

JANS_FILES =  $(JANS)/dump.c $(JANS)/hashtable.c $(JANS)/hashtable_seed.c \
              $(JANS)/load.c $(JANS)/value.c $(JANS)/error.c 
           
POPEN_FILES = $(POPEN)/popen_noshell.c

APREQ_FILES = $(APREQ)/apache_cookie.c $(APREQ)/apache_multipart_buffer.c \
              $(APREQ)/apache_request.c 
              
#   the default target
all: mod_dart

# main module target 
.PHONY: mod_dart
mod_dart: 
          
	$(APXS) -a -c -Wc,-Wall $(UBUNTU) -Wi, \
			$(SRC_FILES) \
	      -I$(CTMPL)  $(TEMPL_FILES) \
	      -I$(JANS)   $(JANS_FILES) \
	      -I$(POPEN)  $(POPEN_FILES) \
	      -I$(APREQ)  $(APREQ_FILES)


#   cleanup
clean:
	rm -f $(SRC)/.libs/* rm $(APREQ)/*.lo rm $(APREQ)/*.la rm $(APREQ)/*.slo rm $(APREQ)/*.o \
	rm $(POPEN)/*.lo rm $(POPEN)/*.la rm $(POPEN)/*.slo rm $(POPEN)/*.o \
	rm $(JANS)/*.lo rm $(JANS)/*.la rm $(JANS)/*.slo rm $(JANS)/*.o \
	rm $(CTMPL)/*.lo rm $(CTMPL)/*.la rm $(CTMPL)/*.slo rm $(CTMPL)/*.o \
	rm $(SRC)/*.lo rm $(SRC)/*.la rm $(SRC)/*.slo rm $(SRC)/*.o 
	
	