# Session Handling 

The session implementation of mod_dart follows that of PHP albeit in a much simpler manner.

There is a Session superglobal supported by the methods startSession and endSession.
Session support is provided by the Apache session modules family, mod_session must be present,
to use cookie based session handling module mod_session_cookie must be present and to use
a MySQL backend the module mod_session_dbd must be present. These are normally all installed
together to provide full session support for Apache.

To use sessions in mod_dart simply call startSession, note this as per PHP needs to be called
on every page invocation of your Dart scripts, to end sessions just call endSession.

Other than that setup is as standard Apache, for example to use browser based cookie
sessions add this to your vhost :-

```
 Session On
 SessionCookieName dartsession path=/
```

All other Apache session directives are supported, refer to the Apache documentation.

An example of usage can be found in the index.dart file in the test directory.