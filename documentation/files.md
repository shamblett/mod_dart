# File/Multi-part form Handling 

The Files implementation of mod_dart follows that of the PHP super-global $_FILES.
It is complete other than the 'error' field, this is not supported by mod-dart.

Input fields of a multi part form of the 'file' type are uploaded to the server and
placed in the cache directory specified in the mod-dart directives. They are specially
named files, to get the uploaded file please use the moveUploadedFile method of the 
Apache class, this operates as per the PHP move_uploaded_file function.

Input variables other than type 'file' are parsed and placed in the Post super-global
as per PHP.

Note that mod-dart does not yet support a max_upload_size, so will attempt to upload
any size file, this facility will be added in a later release.

The file form_file.html can be used to test the Files functionality, this can be found
[here](http://moddart.no-ip.net/form_file.html).

