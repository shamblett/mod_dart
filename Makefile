   ##
##  Makefile -- Build procedure for the mod_dart Apache module
##  S. Hamblett Feb 2015
##

builddir=.
home=/home/steve

dart_src=$(home)/Development/google/dart/build/dart
dart_arch=X64
dart_mode=Release
dart_lib=$(dart_src)/out/$(dart_mode)$(dart_arch)/obj.target/runtime
dart_libnative=$(dart_src)/out/$(dart_mode)$(dart_arch)/obj.target/libdart/runtime/vm
dart_libbuiltinbin=$(dart_src)/out/$(dart_mode)$(dart_arch)/obj.target/dart_no_snapshot/runtime/bin
dart_libbuiltingen=$(dart_src)/out/$(dart_mode)$(dart_arch)/obj.target/dart_no_snapshot/gen
dart_ssl=$(dart_src)/out/$(dart_mode)$(dart_arch)/obj.target/libssl_dart/third_party/net_nss/ssl
dart_gen=$(dart_src)/out/$(dart_mode)$(dart_arch)/obj.target/gen
dart_mod_src=../../
apr_lib=/home/steve/rpmbuild/BUILD/apr-util-1.5.2/.libs/

#   the used tools
APXS=apxs

#   additional defines, includes and libraries
COPTS="-DNDEBUG"
LTFLAGS="--tag=CC"

#   the default target
all: tmpl dart

# template target
.PHONY: tmpl
tmpl: 
	
	python $(dart_src)/runtime/tools/create_string_literal.py --output mod_dart_gen.c --include 'none' --input_cc mod_dart_gen.c.tmpl --var_name "mod_dart_source" $(dart_mod_src)/mod_dart.dart

# main module target 
.PHONY: dart
dart: 
	
	$(APXS) -S CC=g++  -c $(COPTS) -o mod_dart.so -Wc,-Wall   -Wi, -I $(dart_src)/runtime -lstdc++ -I $(dart_gen) \
          -Wl, $(dart_libbuiltingen)/io_patch_gen.o $(dart_libbuiltingen)/io_gen.o $(dart_libbuiltingen)/builtin_gen.o $(dart_libbuiltinbin)/builtin.o $(dart_libbuiltinbin)/builtin_natives.o $(dart_lib)/libdart_vm.a $(dart_lib)/libdart_lib_withcore.a  $(dart_lib)/libdart.a  $(dart_lib)/libdart_lib.a \
        $(dart_lib)/libdouble_conversion.a  $(dart_lib)/libdart_io.a  $(dart_lib)/libdart_vm.a $(dart_lib)/libdart_lib_withcore.a $(dart_lib)/libdouble_conversion.a \
        $(dart_lib)/libdart_builtin.a $(dart_lib)/libjscre.a  \
        apache.c mod_dart_gen.c mod_dart.c


#   cleanup
clean:
	rm -f .libs/* rm mod_dart_gen.c
