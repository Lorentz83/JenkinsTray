JenkinsTray
===========

This is a system tray icon to monitor the
[jenkins](https://jenkins-ci.org/) continuous integration system.

It is written using QT5 and tested on Linux, but should work on Mac
and Windows too.

**How it works**

Simply run `JenkinsTray`. A new system tray will appear on your
desktop. You can right click on it and configure the polling time and
the Jenkins URL.

You can either pass the required parameters on the command line

```
JenkinsTray [url] [polling time (sec)] [t|f]
```

where the last boolean parameter specifies if the ssl errors must be
ignored (useful if you run Jenkins on https using a self signed
certificate) 
