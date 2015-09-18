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

You can specify on the command line the number of seconds to wait for
a system tray. It is useful if you run JenkinsTray in an autostart
script that executes before your desktop system tray is available.

```
JenkinsTray [ -w [secs] ]
```
if you do not specify the seconds, or specify 0, the program will wait
indefinitely.
